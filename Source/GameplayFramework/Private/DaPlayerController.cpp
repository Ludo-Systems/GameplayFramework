// Fill out your copyright notice in the Description page of Project Settings.


#include "DaPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/DaAbilitySystemComponent.h"
#include "Blueprint/UserWidget.h"
#include "DaInputComponent.h"
#include "Kismet/GameplayStatics.h"


ADaPlayerController::ADaPlayerController()
{
	bReplicates = true;
}

void ADaPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(InputMappingContext);
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->ClearAllMappings();
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}
}

void ADaPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	check(InputConfig);
	
	UDaInputComponent* IC = CastChecked<UDaInputComponent>(InputComponent);
	IC->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);

}

void ADaPlayerController::AnyKeyInput(FKey PressedKey)
{
	bIsUsingGamepad = PressedKey.IsGamepadKey();
}

void ADaPlayerController::AbilityInputTagPressed(const FInputActionValue& Value, FGameplayTag InputTag)
{
	//LogOnScreen(this, FString::Printf(TEXT("Input Pressed: %s"), *InputTag.ToString()), true, FColor::Red, 1.f, 1 );
	//bIsUsingGamepad = PressedKey.IsGamepadKey();

	if (GetAbilitySystemComponent() == nullptr) return;
	GetAbilitySystemComponent()->AbilityInputTagPressed(Value, InputTag);
}

void ADaPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	//LogOnScreen(this, FString::Printf(TEXT("Input Released: %s"), *InputTag.ToString()), true, FColor::Blue, 1.f, 2 );
	if (GetAbilitySystemComponent() == nullptr) return;
	GetAbilitySystemComponent()->AbilityInputTagReleased(InputTag);
}

void ADaPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	//LogOnScreen(this, FString::Printf(TEXT("Input Held: %s"), *InputTag.ToString()), true, FColor::Green, 1.f, 3 );
	if (GetAbilitySystemComponent() == nullptr) return;
	GetAbilitySystemComponent()->AbilityInputTagHeld(InputTag);
}

UDaAbilitySystemComponent* ADaPlayerController::GetAbilitySystemComponent()
{
	if (AbilitySystemComponent == nullptr)
	{
		AbilitySystemComponent = Cast<UDaAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()) );
	}

	return AbilitySystemComponent;
}

void ADaPlayerController::BeginPlayingState()
{
	Super::BeginPlayingState();

	BlueprintBeginPlayingState();
}

void ADaPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	OnPlayerStateReceived.Broadcast(PlayerState);
}

void ADaPlayerController::TogglePauseMenu()
{
	if (PauseMenuInstance && PauseMenuInstance->IsInViewport())
	{
		PauseMenuInstance->RemoveFromParent();
		PauseMenuInstance = nullptr;

		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());

		//@TODO: Single-player only. Make work for multiplayer.
		// Exapple issues to resolve: triggering abilities while the game is paused, or releasing your sprint button after pausing the game 
		if (GetWorld()->IsNetMode(NM_Standalone))
		{
			UGameplayStatics::SetGamePaused(this, false);
		}
		
		return;
	}
	
	PauseMenuInstance = CreateWidget<UUserWidget>(this, PauseMenuClass);
	if (PauseMenuInstance)
	{
		PauseMenuInstance->AddToViewport(100);

		bShowMouseCursor = true;
		SetInputMode(FInputModeUIOnly());

		//@TODO: Single-player only. Make work for multiplayer
		if (GetWorld()->IsNetMode(NM_Standalone))
		{
			UGameplayStatics::SetGamePaused(this, true);
		}
	}
}
