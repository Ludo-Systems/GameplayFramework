// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "DaBTService_CheckLowHealth.generated.h"

/** UDaBTService_CheckLowHealth
 *
 *  Checks if Actor's health is less than 25%
 *  
 */
UCLASS()
class GAMEPLAYFRAMEWORK_API UDaBTService_CheckLowHealth : public UBTService
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category="AI")
	FBlackboardKeySelector LowHealthKey;
	
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
