// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Effects/DaGameplayEffect_HealToMax.h"

#include "AbilitySystem/Attributes/DaCharacterAttributeSet.h"
#include "CoreGameplayTags.h"

UDaGameplayEffect_HealToMax::UDaGameplayEffect_HealToMax()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;
	
	FGameplayModifierInfo Modifier;
	Modifier.Attribute = UDaCharacterAttributeSet::GetHealingAttribute();
	FSetByCallerFloat MagnitudeData = FSetByCallerFloat();
	MagnitudeData.DataTag = CoreGameplayTags::TAG_Event_Heal;
	Modifier.ModifierMagnitude = FGameplayEffectModifierMagnitude(MagnitudeData);
	Modifiers.Add(Modifier);
}
