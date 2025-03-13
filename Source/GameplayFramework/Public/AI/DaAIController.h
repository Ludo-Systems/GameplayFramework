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
	ADaAIController();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY(BlueprintReadOnly, Category="AI")
	bool bHasAddedAbilitySystemComponentToBlackboard;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AI")
	bool bAutoRunBehaviorTree;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AI")
	FName TargetActorKeyName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AI")
	FName AbilitySystemComponentKeyName;
	
	UFUNCTION(BlueprintCallable, Category = "AI")
	void AddPlayerPawnAbilitySystemComponentToBlackboard();

	UFUNCTION(BlueprintCallable, Category = "AI")
	void AddTargetActorToBlackBoard(AActor* TargetActor);
	
	virtual void BeginPlay() override;
	
};
