// Copyright Dream Awake Solutions LLC

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "DaBTTask_DoAbilityToSelf.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPLAYFRAMEWORK_API UDaBTTask_DoAbilityToSelf : public UBTTaskNode
{
	GENERATED_BODY()

public:

	UDaBTTask_DoAbilityToSelf();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:

	UPROPERTY(EditAnywhere, Category="AbilitySystem")
	FGameplayTag AbilityToExecute;
};
