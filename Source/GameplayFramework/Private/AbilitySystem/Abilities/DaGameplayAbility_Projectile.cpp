// Copyright Dream Awake Solutions LLC


#include "AbilitySystem/Abilities/DaGameplayAbility_Projectile.h"

#include "CoreGameplayTags.h"
#include "DaCharacterInterface.h"
#include "DaProjectile.h"
#include "GameplayFramework.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_SpawnActor.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystem/Tasks/DaAbilityTask_TargetDataUnderCursor.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

UDaGameplayAbility_Projectile::UDaGameplayAbility_Projectile(
	const FObjectInitializer& ObjectInitializer)
		: Super(ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;

	WaitHitDelay = 0.5f;
	
	SweepRadius = 20.0f;
	SweepDistanceFallback = 5000;

	HitType = ETargetLocationHitType::MouseCursor;
}

void UDaGameplayAbility_Projectile::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                    const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                    const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{			
		constexpr bool bReplicateEndAbility = true;
		constexpr bool bWasCancelled = true;
		EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	}
	
	if (GetCurrentActorInfo()->IsNetAuthority())
	{
		// Allow projectile to fire and only end the ability after some time, to give any subclasses time to respond to events on hit or overlap.
		// need to also end here in case they never hit
		FTimerDelegate Delegate;
		Delegate.BindUObject(this, &ThisClass::EndAbility, Handle, ActorInfo, ActivationInfo, true, false);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_EndDelay, Delegate, WaitHitDelay, false);
	}
}

void UDaGameplayAbility_Projectile::PlayMontageAndSpawnProjectile()
{
	// even if the ability ends, should complete the attack montage. 
	UAnimMontage* Montage = IDaCharacterInterface::Execute_GetAttackMontage(GetAvatarActorFromActorInfo());
	UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, "", Montage, 1.0f, NAME_None, false);
	UAbilityTask_WaitGameplayEvent* EventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, CoreGameplayTags::TAG_Event_Montage_Projectile, nullptr, true);
	EventTask->EventReceived.AddDynamic(this, &ThisClass::AnimNotifyEventCallback);
	EventTask->Activate();
	MontageTask->Activate();
}

void UDaGameplayAbility_Projectile::AnimNotifyEventCallback(FGameplayEventData Payload)
{
	LOG("GameplayAbility Projectile: AnimNotify Event received")
	
	CalculateTargetLocationAndSpawnProjectile();
}

void UDaGameplayAbility_Projectile::CalculateTargetLocationAndSpawnProjectile()
{
	if (HitType == ETargetLocationHitType::SphereTrace)
	{
		FVector ProjectileTargetLocation = GetTargetLocation_SphereTrace();
		SpawnProjectile(ProjectileTargetLocation);
	}
	else if (HitType == ETargetLocationHitType::MouseCursor)
	{
		// Trace using TargetData cursor task
		UDaAbilityTask_TargetDataUnderCursor* TargetDataUnderCursorTask = UDaAbilityTask_TargetDataUnderCursor::CreateTargetDataUnderCursor(this);
		TargetDataUnderCursorTask->ValidData.AddDynamic(this, &ThisClass::OnCursorTargetLocationReceived);
		TargetDataUnderCursorTask->Activate();
	}
}

void UDaGameplayAbility_Projectile::OnCursorTargetLocationReceived(const FGameplayAbilityTargetDataHandle& DataHandle)
{
	LogOnScreen(this,FString::Printf(TEXT("UDaGameplayAbility_Projectile: DataHandle valid?: %hhd."), DataHandle.IsValid(0)), true, FColor::Yellow, 5.f);
	// FIXME: This currently fails when clicked on Client
	if (DataHandle.IsValid(0))
	{
		const FHitResult* HitResult = DataHandle.Get(0)->GetHitResult();
		FVector ProjectileTargetLocation = HitResult->Location;
		SpawnProjectile(ProjectileTargetLocation);
	}
}

FVector UDaGameplayAbility_Projectile::GetTargetLocation_SphereTrace()
{
	ACharacter* InstigatorCharacter = Cast<ACharacter>(GetAvatarActorFromActorInfo());
	if (!InstigatorCharacter) return FVector();

	FVector AdjustedTraceEnd;
	if (HitType != ETargetLocationHitType::SphereTrace)
	{
		LOG_ERROR("Invalid GetTargetLocation_SphereTrace Calculation. Must set HitType==ETargetLocationHitType::SphereTrace to use GetTargetLocation_SphereTrace.")
		return AdjustedTraceEnd;
	}
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = InstigatorCharacter;

	// Trace against the environment to find whats under the player crosshair.
	// We use the hit location to adjust the projectile launch direction so
	// it will hit what is under the crosshair rather than aim directly forward from the player's socket.

	FCollisionShape Shape;
	Shape.SetSphere(SweepRadius);

	// Ignore Player
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(InstigatorCharacter);

	FVector TraceDirection = InstigatorCharacter->GetControlRotation().Vector();

	// Add sweep radius onto start to avoid the sphere clipping into floor/walls the camera is directly against.
	const FVector TraceStart = InstigatorCharacter->GetPawnViewLocation() + (TraceDirection * SweepRadius);
	// endpoint far into the look-at distance (not too far, still adjust somewhat towards crosshair on a miss)
	const FVector TraceEnd = TraceStart + (TraceDirection * SweepDistanceFallback);
	AdjustedTraceEnd = TraceEnd;

	TArray<FHitResult> Hits;
	// Enemies are using Overlap response to Projectiles, we cant look for single Blocking hits and instead look for all overlaps and filter after
	if (GetWorld()->SweepMultiByChannel(Hits, TraceStart, TraceEnd, FQuat::Identity, COLLISION_PROJECTILE, Shape, Params))
	{
		// Overwrite trace end with impact point in world
		// First entry must exist and first entry will be first overlap or block
		// Could filter further, eg. ignoring friendly players between us and the enemy
		AdjustedTraceEnd = Hits[0].ImpactPoint;
	}
	
	return AdjustedTraceEnd;
}

void UDaGameplayAbility_Projectile::SpawnProjectile(const FVector& ProjectileTargetLocation)
{
	if (ACharacter* InstigatorCharacter = Cast<ACharacter>(GetAvatarActorFromActorInfo()))
	{
		const FVector ProjectileLaunchLocation = IDaCharacterInterface::Execute_GetProjectileSocketLocation(InstigatorCharacter);
		UGameplayStatics::SpawnEmitterAtLocation(this, CastingEffect, ProjectileLaunchLocation);
		UGameplayStatics::SpawnSoundAtLocation(this, CastingSound, ProjectileLaunchLocation);
	
		if (InstigatorCharacter->HasAuthority() && ensureAlways(ProjectileClass))
		{
			FRotator Rotation = (ProjectileTargetLocation - ProjectileLaunchLocation).Rotation();
			Rotation.Pitch = PitchAdjustmentMultiplier*Rotation.Pitch;

			FTransform SpawnTransform;
			SpawnTransform.SetLocation(ProjectileLaunchLocation);
			SpawnTransform.SetRotation(Rotation.Quaternion());

			ADaProjectile* Projectile = GetWorld()->SpawnActorDeferred<ADaProjectile>(ProjectileClass,
			SpawnTransform,
			InstigatorCharacter,
			InstigatorCharacter,
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

			Projectile->FinishSpawning(SpawnTransform);
		}
	}
}

void UDaGameplayAbility_Projectile::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                               const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                               bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	// Ended in subclasses invalidate timer
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_EndDelay);
}
