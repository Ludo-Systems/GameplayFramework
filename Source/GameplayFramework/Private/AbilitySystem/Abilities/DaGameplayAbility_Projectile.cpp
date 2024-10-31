// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/DaGameplayAbility_Projectile.h"

#include "GameplayFramework.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_SpawnActor.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

UDaGameplayAbility_Projectile::UDaGameplayAbility_Projectile(
	const FObjectInitializer& ObjectInitializer)
		: Super(ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;

	AttackAnimDelay = 0.2f;
	WaitHitDelay = 0.5f;
	
	SweepRadius = 20.0f;
	SweepDistanceFallback = 5000;
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

	// even if the ability ends, should complete the attack montage. 
	ACharacter* Character = CastChecked<ACharacter>(GetAvatarActorFromActorInfo());

	if (AttackAnim)
	{
		UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, "", AttackAnim, 1.0f, NAME_None, false);
		MontageTask->Activate();
	}
	
	if (!HandSocketName.IsNone())
	{
		UGameplayStatics::SpawnEmitterAttached(CastingEffect, Character->GetMesh(), HandSocketName, FVector::ZeroVector, FRotator::ZeroRotator,
			EAttachLocation::SnapToTarget, true, EPSCPoolMethod::AutoRelease);
	}
	else
	{
		UGameplayStatics::SpawnEmitterAttached(CastingEffect, Character->GetCapsuleComponent(), FName(), FVector::ZeroVector, FRotator::ZeroRotator,
			EAttachLocation::SnapToTarget, true, EPSCPoolMethod::AutoRelease);
	}
	
	UGameplayStatics::SpawnSoundAttached(CastingSound, Character->GetMesh());

	if (GetCurrentActorInfo()->IsNetAuthority())
	{
		FTimerHandle TimerHandle_AttackDelay;
		FTimerDelegate Delegate;
		Delegate.BindUObject(this, &ThisClass::AttackDelay_Elapsed, Character, Handle, ActorInfo, ActivationInfo);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay, Delegate, AttackAnimDelay, false);
	}
	else
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	}
}

void UDaGameplayAbility_Projectile::AttackDelay_Elapsed(ACharacter* InstigatorCharacter, const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	// Blueprint has not been properly configured yet if this fails
	if (ensureAlways(ProjectileClass))
	{
		FVector HandLocation = InstigatorCharacter->GetMesh()->GetSocketLocation(HandSocketName);

		// We trace against the environment first to find whats under the player crosshair.
		// We use the hit location to adjust the projectile launch direction so it will hit what is under the crosshair rather than shoot straight forward from the player hands.

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = InstigatorCharacter;

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
		FVector AdjustedTraceEnd = TraceEnd;

		TArray<FHitResult> Hits;
		// Enemies are using Overlap response to Projectiles, we cant look for single Blocking hits and instead look for all overlaps and filter after
		if (GetWorld()->SweepMultiByChannel(Hits, TraceStart, TraceEnd, FQuat::Identity, COLLISION_PROJECTILE, Shape, Params))
		{
			// Overwrite trace end with impact point in world
			// First entry must exist and first entry will be first overlap or block
			// Could filter further, eg. ignoring friendly players between us and the enemy
			AdjustedTraceEnd = Hits[0].ImpactPoint;
		}
		
		// find new direction/rotation from Hand pointing to impact point in world.
		FRotator ProjRotation = (AdjustedTraceEnd - HandLocation).Rotation();
		FTransform SpawnTM = FTransform(ProjRotation, HandLocation);

		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);

		//TODO: Needed? UAbilityTask_SpawnActor::SpawnActor()
	}

	// Allow projectile to fire and only end the ability after some time, to give any subclasses time to respond to events on hit or overlap.
	// need to also end here in case they never hit
	FTimerDelegate Delegate;
	Delegate.BindUObject(this, &ThisClass::EndAbility, Handle, ActorInfo, ActivationInfo, true, false);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_EndDelay, Delegate, WaitHitDelay, false);
}

void UDaGameplayAbility_Projectile::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	// Ended in subclasses invalidate timer
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_EndDelay);
}
