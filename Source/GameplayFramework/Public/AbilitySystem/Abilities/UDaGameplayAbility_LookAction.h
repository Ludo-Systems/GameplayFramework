// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DaGameplayAbility_BaseTriggeredInputAction.h"
#include "UDaGameplayAbility_LookAction.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPLAYFRAMEWORK_API UUDaGameplayAbility_LookAction : public UDaGameplayAbility_BaseTriggeredInputAction
{
	GENERATED_BODY()

	virtual void OnTriggeredInputAction(const FInputActionValue& Value) override;

public:
	UUDaGameplayAbility_LookAction(const FObjectInitializer& ObjectInitializer);
	
};
