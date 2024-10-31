// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "DaBTService_CheckAttackRange.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPLAYFRAMEWORK_API UDaBTService_CheckAttackRange : public UBTService
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(EditAnywhere, Category="AI")
	FBlackboardKeySelector AttackRangeKey;

	UPROPERTY(VisibleAnywhere, Category="AI")
	FName TargetActorKeyName;
	
	UPROPERTY(EditAnywhere, Category="AI")
	float TraceDistance;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:
	
	UDaBTService_CheckAttackRange();
	
};
