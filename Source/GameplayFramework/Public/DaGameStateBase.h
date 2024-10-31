// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/GameStateBase.h"
#include "DaGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPLAYFRAMEWORK_API ADaGameStateBase : public AGameStateBase, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

	ADaGameStateBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// Gets the ability system component used for game wide things
	UFUNCTION(BlueprintCallable, Category = "DA|GameState")
	UDaAbilitySystemComponent* GetLyraAbilitySystemComponent() const { return AbilitySystemComponent; }

private:

	// The ability system component subobject for game-wide things (primarily gameplay cues)
	UPROPERTY(VisibleAnywhere, Category = "DA|GameState")
	TObjectPtr<UDaAbilitySystemComponent> AbilitySystemComponent;

	virtual void PostInitializeComponents() override;
};
