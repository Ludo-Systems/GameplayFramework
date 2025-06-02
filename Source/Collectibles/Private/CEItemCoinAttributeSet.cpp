// Fill out your copyright notice in the Description page of Project Settings.


#include "CEItemCoinAttributeSet.h"

#include "CollectiblesGameplayTags.h"
#include "Net/UnrealNetwork.h"

UCEItemCoinAttributeSet::UCEItemCoinAttributeSet()
	: AppraisedGrade(0.f), DerivedValue(0.f)
{
	SetIdentifierTag = CollectiblesGameplayTags::TAG_AttributesStatsCollectible;

	// Primary
	TagsToAttributes.Add(CollectiblesGameplayTags::TAG_AttributesStatsPrimaryCollectible_AppraisedGrade, GetAppraisedGradeAttribute);
	
	// Secondary
	TagsToAttributes.Add(CollectiblesGameplayTags::TAG_AttributesStatsSecondaryCollectible_DerivedValue, GetDerivedValueAttribute);

}

void UCEItemCoinAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Primary Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UCEItemCoinAttributeSet, AppraisedGrade, COND_None, REPNOTIFY_Always);
	
	// Secondary Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UCEItemCoinAttributeSet, DerivedValue, COND_None, REPNOTIFY_Always);
}

void UCEItemCoinAttributeSet::OnRep_AppraisedGrade(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCEItemCoinAttributeSet, AppraisedGrade, OldValue);

	const float CurrentAppraisedGrade = GetAppraisedGrade();
	const float EstimatedMagnitude = CurrentAppraisedGrade - OldValue.GetCurrentValue();
	
	OnAppraisedGradeChanged.Broadcast(nullptr, nullptr, nullptr, EstimatedMagnitude, OldValue.GetCurrentValue(), CurrentAppraisedGrade);
}

void UCEItemCoinAttributeSet::OnRep_DerivedValue(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCEItemCoinAttributeSet, DerivedValue, OldValue);

	const float CurrentDerivedValue = GetDerivedValue();
	const float EstimatedMagnitude = CurrentDerivedValue - OldValue.GetCurrentValue();
	
	OnDerivedValueChanged.Broadcast(nullptr, nullptr, nullptr, EstimatedMagnitude, OldValue.GetCurrentValue(), CurrentDerivedValue);
}

bool UCEItemCoinAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	return Super::PreGameplayEffectExecute(Data);
}

void UCEItemCoinAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
}

void UCEItemCoinAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	ClampAttribute(Attribute, NewValue);

}

void UCEItemCoinAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	ClampAttribute(Attribute, NewValue);
}

void UCEItemCoinAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	// Set Secondary attribute DerivedValue here based on curve lookup table with Grade as Input
}

void UCEItemCoinAttributeSet::ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetAppraisedGradeAttribute())
	{
		// Unset Grade will be 0, Set Grade Values must be between 1-10
		NewValue = FMath::Clamp<int32>(NewValue, 0.0f, 10.0f);
	}
	else
	{
		// Dont allow negative numbers
		NewValue = FMath::Max(NewValue, 0.0f);
	}
}


