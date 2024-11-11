// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "DaAbilitySystemComponent.generated.h"

class UDaAbilitySet;
struct FInputActionValue;
class UDaInputConfig;
struct FDaAbilitySet_GrantedHandles;
class UDaPawnData;

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer& /*AssetTags*/)
/**
 * 
 */
UCLASS()
class GAMEPLAYFRAMEWORK_API UDaAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

protected:

	UPROPERTY()
	TArray<FDaAbilitySet_GrantedHandles> OutGrantedAbilityHandlesArray;
	// TODO: Make this a map so we can remove individual AbilitySets later ex. TSet<FDaAbilitySet_GrantedHandles, FGameplayTag> AbilitySets;

	void EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);
	
public:
	UFUNCTION(BlueprintPure, Category = "DA|AbilitySystem")
	static UDaAbilitySystemComponent* FindAbilitySystemComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UDaAbilitySystemComponent>() : nullptr); }

	FEffectAssetTags EffectAssetTags;
	
	void InitAbilitiesWithPawnData(const UDaPawnData* DataAsset);
	void ClearAbilitySets();

	void GrantSet(const UDaAbilitySet* AbilitySet);
	
	void AbilityInputTagPressed(const FInputActionValue& Value, const FGameplayTag& InputTag);
	void AbilityInputTagHeld(const FGameplayTag InputTag);
	void AbilityInputTagReleased(const FGameplayTag InputTag);

	void AbilityActorInfoSet();
};
