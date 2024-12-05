// Copyright Dream Awake Solutions LLC

#pragma once

#include "CoreMinimal.h"
#include "DaGameplayAbilityBase.h"
#include "DaGameplayAbility_BaseTriggeredInputAction.generated.h"

class UDaInputComponent;
struct FInputActionValue;

/**
 * 
 */
UCLASS()
class GAMEPLAYFRAMEWORK_API UDaGameplayAbility_BaseTriggeredInputAction : public UDaGameplayAbilityBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DA|Ability")
	bool bCancelAbilityOnInputReleased;
	
	UDaGameplayAbility_BaseTriggeredInputAction(const FObjectInitializer& ObjectInitializer);

protected:
	UPROPERTY()
	TObjectPtr<UDaInputComponent> InputComponent;

	TArray<uint32> TriggeredEventHandles;

	// UGameplayAbility Interface
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	
	// Subclasses should override this
	UFUNCTION(BlueprintNativeEvent)
	void OnTriggeredInputAction(const FInputActionValue& Value);
	
};
