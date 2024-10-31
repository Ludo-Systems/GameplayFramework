// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "DaBTTask_DoAbilityToTargetActor.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPLAYFRAMEWORK_API UDaBTTask_DoAbilityToTargetActor : public UBTTaskNode
{
	GENERATED_BODY()

public:

	UDaBTTask_DoAbilityToTargetActor();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:

	UPROPERTY(EditAnywhere, Category="AbilitySystem")
	FGameplayTag AbilityToExecute;

	UPROPERTY(VisibleAnywhere, Category="AbilitySystem")
	FName TargetActorKeyName;
};
