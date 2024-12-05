// Copyright Dream Awake Solutions LLC

#pragma once

#include "CoreMinimal.h"
#include "DaGameplayAbility_BaseTriggeredInputAction.h"
#include "UDaGameplayAbility_MoveAction.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPLAYFRAMEWORK_API UUDaGameplayAbility_MoveAction : public UDaGameplayAbility_BaseTriggeredInputAction
{
	GENERATED_BODY()

	virtual void OnTriggeredInputAction_Implementation(const FInputActionValue& Value) override;

public:
	UUDaGameplayAbility_MoveAction(const FObjectInitializer& ObjectInitializer);
	
};
