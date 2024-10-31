// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DaBaseAttributeSet.h"
#include "AbilitySystemComponent.h"

#include "DaCharacterAttributeSet.generated.h"

class UObject;

/**
 * 
 */
UCLASS()
class GAMEPLAYFRAMEWORK_API UDaCharacterAttributeSet : public UDaBaseAttributeSet
{
	GENERATED_BODY()

public:

	UDaCharacterAttributeSet();

	ATTRIBUTE_ACCESSORS(UDaCharacterAttributeSet, Health);
	ATTRIBUTE_ACCESSORS(UDaCharacterAttributeSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(UDaCharacterAttributeSet, Mana);
	ATTRIBUTE_ACCESSORS(UDaCharacterAttributeSet, MaxMana);
	
	// meta
	ATTRIBUTE_ACCESSORS(UDaCharacterAttributeSet, Healing);
	ATTRIBUTE_ACCESSORS(UDaCharacterAttributeSet, Damage);

	// Delegate when health changes due to damage/healing, some information may be missing on the client
	mutable FDaAttributeEvent OnHealthChanged;

	// Delegate when max health changes
	mutable FDaAttributeEvent OnMaxHealthChanged;

	// Delegate to broadcast when the health attribute reaches zero
	mutable FDaAttributeEvent OnOutOfHealth;

	// Delegate when Mana changed
	mutable FDaAttributeEvent OnManaChanged;

	// Delegate when max mana changes
	mutable FDaAttributeEvent OnMaxManaChanged;
	
protected:
	
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldValue);

	virtual bool PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const;
	
private:
	
	// The current health attribute.  The health will be capped by the max health attribute.  Health is hidden from modifiers so only executions can modify it.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "DA|Vitals", Meta = (HideFromModifiers, AllowPrivateAccess = true)) 
	FGameplayAttributeData Health;

	// The current max health attribute.  Max health is an attribute since gameplay effects can modify it.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "DA|Vitals", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth;

	// Mana
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "DA|Vitals", Meta = (AllowPrivateAccess = true)) 
	FGameplayAttributeData Mana;
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "DA|Vitals", Meta = (AllowPrivateAccess = true)) 
	FGameplayAttributeData MaxMana;
	
	// Used to track when the health reaches 0.
	bool bOutOfHealth;

	// Store the attributes before any changes 
	float MaxHealthBeforeAttributeChange;
	float HealthBeforeAttributeChange;
	float MaxManaBeforeAttributeChange;
	float ManaBeforeAttributeChange;
	// -------------------------------------------------------------------
	//	Meta Attribute (please keep attributes that aren't 'stateful' below 
	// -------------------------------------------------------------------
	
	// Incoming healing. This is mapped directly to +Health
	UPROPERTY(BlueprintReadOnly, Category="DA|Health", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData Healing;

	// Incoming damage. This is mapped directly to -Health
	UPROPERTY(BlueprintReadOnly, Category="DA|Health", Meta=(AllowPrivateAccess=true)) /* @TODO: Create Execution and use HideFromModifiers */
	FGameplayAttributeData Damage;
	
};
