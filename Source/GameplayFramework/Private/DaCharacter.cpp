// Fill out your copyright notice in the Description page of Project Settings.


#include "DaCharacter.h"

#include "Components/CapsuleComponent.h"
#include "AbilitySystem/DaAbilitySystemComponent.h"
#include "DaAttributeComponent.h"
#include "DaPlayerState.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CoreGameplayTags.h"
#include "DaPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "UI/DaHUD.h"
#include "UI/DaWorldUserWidget.h"
#include "Util/ColorConstants.h"

// Sets default values
ADaCharacter::ADaCharacter()
{
	CharacterTypeGameplayTag = CoreGameplayTags::TAG_Character_Type_Player;

	PrimaryActorTick.bCanEverTick = false;
	
	AttributeComp = CreateDefaultSubobject<UDaAttributeComponent>(TEXT("AttributeComponent"));
	
	// Collision settings. We are only interested in handling specific overlap events on the character mesh and not the capsule and not after any animations.
	GetMesh()->bUpdateOverlapsOnAnimationFinalize = false;
	GetMesh()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
}

void ADaCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//server
	InitAbilitySystem();
}

void ADaCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	//client
	InitAbilitySystem();
}

void ADaCharacter::InitAbilitySystem()
{
	ADaPlayerState* PS = GetPlayerState<ADaPlayerState>();
	if (PS)
	{
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);
		AbilityComp = PS->GetDaAbilitySystemComponent();

		// Load AbilitySet and any other pawn data for this character
		// Will setup the ability system component with Abilities, Effects, and needed Attributes
		PS->InitializePlayerPawnData();

		// Attribute Component set in PlayerState after loading pawn data
		AttributeComp->OnHealthChanged.AddDynamic(this, &ADaCharacter::OnHealthChanged);
		AttributeComp->OnDeathStarted.AddDynamic(this, &ADaCharacter::OnDeathStarted);
		AttributeComp->OnDeathFinished.AddDynamic(this, &ADaCharacter::OnDeathFinished);
	}
}

void ADaCharacter::UnPossessed()
{
	// server only
	Super::UnPossessed();
}

UAbilitySystemComponent* ADaCharacter::GetAbilitySystemComponent() const
{
	return Cast<UAbilitySystemComponent>(AbilityComp); 
}

void ADaCharacter::InitPlayerHUD() const
{
	ADaPlayerState* PS = GetPlayerState<ADaPlayerState>();
	check(PS);

	ADaPlayerController* PlayerController = Cast<ADaPlayerController>(GetController());
	if (PlayerController && IsLocallyControlled())
	{
		if (ADaHUD* HUD = Cast<ADaHUD>(PlayerController->GetHUD()))
		{
			HUD->RemoveOverlay();
			HUD->InitOverlay(PlayerController, PS, AbilityComp, AttributeComp);
		}
	}
}

void ADaCharacter::OnHealthChanged(UDaAttributeComponent* HealthComponent, float OldHealth, float NewHealth,
	AActor* InstigatorActor)
{
	float Delta = NewHealth - OldHealth;
	if (Delta != 0.0f)
	{

		if (UDaAttributeComponent::IsActorAlive(this))
		{
			ShowSetHealthBarWidget();
			ShowDamagePopupWidget(Delta);
		}
		
		if (bUseDefaultHitFlash)
		{
			// If using our Material function, set delta time to trigger the hit flash, then Green if healing or red if damaged
			GetMesh()->SetScalarParameterValueOnMaterials(HitFlashTimeParamName, GetWorld()->TimeSeconds);
			GetMesh()->SetVectorParameterValueOnMaterials(HitFlashColorParamName, Delta > 0 ? FVector(UE::Geometry::LinearColors::Green3f()) : FVector(UE::Geometry::LinearColors::Red3f()));
		}
	}
}

void ADaCharacter::OnDeathStarted(AActor* OwningActor, AActor* InstigatorActor)
{
	// Death
	if(APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		DisableInput(PC);
	}
	
	// Ragdoll
	// could also use anim bp, but will code it here.
	GetMesh()->SetAllBodiesSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName("Ragdoll");

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->DisableMovement();
}

void ADaCharacter::OnDeathFinished(AActor* OwningActor)
{
	SetLifeSpan(3.0f);
}




