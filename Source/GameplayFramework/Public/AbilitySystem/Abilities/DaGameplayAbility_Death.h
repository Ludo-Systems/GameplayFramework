// Copyright Dream Awake Solutions LLC

#pragma once

#include "CoreMinimal.h"
#include "DaGameplayAbilityBase.h"
#include "DaGameplayAbility_Death.generated.h"

class UDaAttributeComponent;
struct FGameplayAbilityActorInfo;
struct FGameplayEventData;

/**
 * 
 */
UCLASS()
class GAMEPLAYFRAMEWORK_API UDaGameplayAbility_Death : public UDaGameplayAbilityBase
{
	GENERATED_BODY()

public:
	UDaGameplayAbility_Death(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	// Starts the death sequence.
	UFUNCTION(BlueprintCallable, Category = "DA|Ability")
	bool StartDeath(UDaAttributeComponent* HealthComponent, AActor* InstigatorActor);

	// Finishes the death sequence.
	UFUNCTION(BlueprintCallable, Category = "DA|Ability")
	void FinishDeath();

	// If enabled, the ability will automatically call StartDeath.  FinishDeath is always called when the ability ends if the death was started.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DA|Death")
	bool bAutoStartDeath;

private: 
	void FinishDeathAndEndAbility(UDaAttributeComponent* HealthComponent, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled);
	
};
