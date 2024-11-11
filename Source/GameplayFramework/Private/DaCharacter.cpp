// Fill out your copyright notice in the Description page of Project Settings.


#include "DaCharacter.h"

#include "Components/CapsuleComponent.h"
#include "AbilitySystem/DaAbilitySystemComponent.h"
#include "DaAttributeComponent.h"
#include "DaPlayerState.h"
#include "CoreGameplayTags.h"
#include "DaInteractionComponent.h"
#include "DaPlayerController.h"
#include "UI/DaHUD.h"

// Sets default values
ADaCharacter::ADaCharacter()
{
	CharacterTypeGameplayTag = CoreGameplayTags::TAG_Character_Type_Player;

	InteractionComp = CreateDefaultSubobject<UDaInteractionComponent>(TEXT("InteractionComp"));

	PrimaryActorTick.bCanEverTick = false;
	
	// Collision settings. We are only interested in handling specific overlap events on the character mesh and not the capsule and not after any animations.
	GetMesh()->bUpdateOverlapsOnAnimationFinalize = false;
	//GetMesh()->SetGenerateOverlapEvents(true); <-- set in superclass
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
}

void ADaCharacter::InitAbilitySystem()
{
	ADaPlayerState* PS = GetPlayerState<ADaPlayerState>();
	if (PS)
	{
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);
		Cast<UDaAbilitySystemComponent>(PS->GetAbilitySystemComponent())->AbilityActorInfoSet();
		AbilitySystemComponent = PS->GetDaAbilitySystemComponent();

		// Load AbilitySet and any other pawn data for this character
		// Will setup the ability system component with Abilities, Effects, and needed Attributes
		PS->InitializePlayerPawnData();

		// Attribute Component set in PlayerState after loading pawn data
		Super::InitAbilitySystem();

	}
}

void ADaCharacter::PrimaryInteraction() const
{
	if (InteractionComp) InteractionComp->PrimaryInteract();
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
			HUD->InitOverlay(PlayerController, PS, AbilitySystemComponent, AttributeComponent);
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

	// Call super to ragdoll
	Super::OnDeathStarted(OwningActor, InstigatorActor);
}




