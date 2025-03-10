// Copyright Dream Awake Solutions LLC

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "DaAIController.generated.h"

class UBehaviorTree;

UCLASS()
class GAMEPLAYFRAMEWORK_API ADaAIController : public AAIController
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AI")
	bool bAutoRunBehaviorTree = true;
	
	virtual void BeginPlay() override;
	
};
