// Copyright Dream Awake Solutions LLC


#include "AbilitySystem/Effects/DaGameplayEffect_DealDamage.h"

#include "AbilitySystem/Attributes/DaCharacterAttributeSet.h"
#include "CoreGameplayTags.h"

UDaGameplayEffect_DealDamage::UDaGameplayEffect_DealDamage()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;
	
	FGameplayModifierInfo Modifier;
	Modifier.Attribute = UDaCharacterAttributeSet::GetDamageAttribute();
	FSetByCallerFloat MagnitudeData = FSetByCallerFloat();
	MagnitudeData.DataTag = CoreGameplayTags::TAG_Event_Damage;
	Modifier.ModifierMagnitude = FGameplayEffectModifierMagnitude(MagnitudeData);
	Modifiers.Add(Modifier);
}
