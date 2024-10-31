// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/DaAICharacter.h"

#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "CoreGameplayTags.h"
#include "AbilitySystem/DaAbilitySystemComponent.h"
#include "AbilitySystem/Attributes/DaCharacterAttributeSet.h"
#include "AbilitySystem/Attributes/DaCombatAttributeSet.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "DaAttributeComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "UI/DaWorldUserWidget.h"
#include "Util/ColorConstants.h"


// Sets default values
ADaAICharacter::ADaAICharacter()
{
	CharacterTypeGameplayTag = CoreGameplayTags::TAG_Character_Type_AI;
	
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");
	AttributeComp = CreateDefaultSubobject<UDaAttributeComponent>("AttributeComp");
	
	AbilitySystemComponent = CreateDefaultSubobject<UDaAbilitySystemComponent>(TEXT("AbilityComp"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	// These attribute sets will be detected by AbilitySystemComponent::InitializeComponent. Keeping a reference so that the sets don't get garbage collected before that.
	HealthSet = CreateDefaultSubobject<UDaCharacterAttributeSet>(TEXT("HealthSet"));
	CombatSet = CreateDefaultSubobject<UDaCombatAttributeSet>(TEXT("CombatSet"));
	
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetMesh()->SetGenerateOverlapEvents(true);

	TargetActorKey = "TargetActor";
	
	PlayerSeenEmoteTime = 6.0f;
}

void ADaAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	PawnSensingComp->OnSeePawn.AddDynamic(this, &ADaAICharacter::OnPawnSeen);
}

void ADaAICharacter::BeginPlay()
{
	Super::BeginPlay();

	InitAbilitySystem();
}

void ADaAICharacter::InitAbilitySystem()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	AttributeComp->OnHealthChanged.AddDynamic(this, &ADaAICharacter::OnHealthChanged);
	AttributeComp->OnDeathStarted.AddDynamic(this, &ADaAICharacter::OnDeathStarted);
	AttributeComp->OnDeathFinished.AddDynamic(this, &ADaAICharacter::OnDeathFinished);
	
	// read from data asset and set up abilities, attributes, and effects
	AttributeComp->InitializeWithAbilitySystem(AbilitySystemComponent);
}

void ADaAICharacter::OnPawnSeen(APawn* Pawn)
{
	if (GetTargetActor() == Pawn)
	{
		return;
	}
	
	SetTargetActor(Pawn); //@TODO: Toggle mode to set player only vs all characters so game designers can choose

	MulticastOnPawnSeen(Pawn);
}

void ADaAICharacter::MulticastOnPawnSeen_Implementation(APawn* Pawn)
{
	// Add some cool Widget or emote to NPC to signify when they've spotted the player
	if (PlayerSeenWidget == nullptr)
	{
		PlayerSeenWidget = CreateWidget<UDaWorldUserWidget>(GetWorld(), PlayerSeenWidgetClass);
		if (PlayerSeenWidget)
		{
			PlayerSeenWidget->AttachedActor = this;
			PlayerSeenWidget->AddToViewport(10);

			FTimerHandle TimerHandle_PlayerSeenElapsed;
			GetWorldTimerManager().SetTimer(TimerHandle_PlayerSeenElapsed, this, &ADaAICharacter::PlayerSeenWidgetTimeExpired, PlayerSeenEmoteTime );
		}

		FString Msg = FString::Printf(TEXT("%s SPOTTED"), *GetNameSafe(Pawn));
		DrawDebugString(GetWorld(), GetActorLocation(), Msg, nullptr, FColor::White, PlayerSeenEmoteTime, true);
	}
}

void ADaAICharacter::PlayerSeenWidgetTimeExpired()
{
	PlayerSeenWidget->RemoveFromParent();
	PlayerSeenWidget->AttachedActor = nullptr;
	PlayerSeenWidget = nullptr;
}

void ADaAICharacter::OnHealthChanged(UDaAttributeComponent* OwningComp, float OldHealth, float NewHealth, AActor* InstigatorActor)
{
	//LogOnScreen(this, FString::Printf(TEXT("ADaAICharacter::ONHealthChanged: %f, Owner:%s Instigator: %s"), NewHealth, *GetNameSafe(this), *GetNameSafe(InstigatorActor)));
	
	if (NewHealth < OldHealth)
	{
		// Took Damage
		if (InstigatorActor != this)
		{
			SetTargetActor(InstigatorActor);
		}

		if (UDaAttributeComponent::IsActorAlive(this))
		{
			ShowSetHealthBarWidget();
			ShowDamagePopupWidget(NewHealth-OldHealth);
		}
		
		if (bUseDefaultHitFlash)
		{
			GetMesh()->SetScalarParameterValueOnMaterials(HitFlashTimeParamName, GetWorld()->TimeSeconds);
			GetMesh()->SetVectorParameterValueOnMaterials(HitFlashColorParamName, FVector(UE::Geometry::LinearColors::Red3f()));
		}
	}
	else if (NewHealth > OldHealth)
	{
		// Healing
		if (bUseDefaultHitFlash)
		{
			GetMesh()->SetScalarParameterValueOnMaterials(HitFlashTimeParamName, GetWorld()->TimeSeconds);
			GetMesh()->SetVectorParameterValueOnMaterials(HitFlashColorParamName, FVector(UE::Geometry::LinearColors::Green3f()));
		}
	}
}

void ADaAICharacter::OnDeathStarted(AActor* OwningActor, AActor* InstigatorActor)
{
	// stop BT
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		AIController->GetBrainComponent()->StopLogic("Killed");
	}
		
	// Ragdoll
	// could also use anim bp, but will code it here.
	GetMesh()->SetAllBodiesSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName("Ragdoll");

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->DisableMovement();
}

void ADaAICharacter::OnDeathFinished(AActor* OwningActor)
{
	// Lifespan
	SetLifeSpan(5.0f);
}

UAbilitySystemComponent* ADaAICharacter::GetAbilitySystemComponent() const
{
	return Cast<UAbilitySystemComponent>(AbilitySystemComponent);
}

void ADaAICharacter::SetTargetActor(AActor* NewTarget)
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		AIController->GetBlackboardComponent()->SetValueAsObject(TargetActorKey, NewTarget);
	}
}

AActor* ADaAICharacter::GetTargetActor()
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		return Cast<AActor>(AIController->GetBlackboardComponent()->GetValueAsObject(TargetActorKey));
	}
	return nullptr;
}


