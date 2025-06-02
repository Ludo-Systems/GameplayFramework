// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/Attributes/DaBaseAttributeSet.h"
#include "CEItemCoinAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class COLLECTIBLES_API UCEItemCoinAttributeSet : public UDaBaseAttributeSet
{
	GENERATED_BODY()
	
public:

	UCEItemCoinAttributeSet();
	
	// Primary Attributes
	ATTRIBUTE_ACCESSORS(UCEItemCoinAttributeSet, AppraisedGrade);
	
	// Secondary Attributes
	ATTRIBUTE_ACCESSORS(UCEItemCoinAttributeSet, DerivedValue);
	
	mutable FDaAttributeEvent OnAppraisedGradeChanged;
	mutable FDaAttributeEvent OnDerivedValueChanged;

protected:

	// Primary
	UFUNCTION()
	void OnRep_AppraisedGrade(const FGameplayAttributeData& OldValue) const;
	
	// Secondary
	UFUNCTION()
	void OnRep_DerivedValue(const FGameplayAttributeData& OldValue) const;
	
	virtual bool PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	
	void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const;

private:

	// Primary
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_AppraisedGrade, Category = "Primary Attributes", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AppraisedGrade;

	// Secondary
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_DerivedValue, Category = "Secondary Attributes", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData DerivedValue;
	
};
