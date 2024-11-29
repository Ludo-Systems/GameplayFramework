// Copyright Dream Awake Solutions LLC


#include "AbilitySystem/Attributes/DaCombatAttributeSet.h"

#include "CoreGameplayTags.h"
#include "Net/UnrealNetwork.h"

UDaCombatAttributeSet::UDaCombatAttributeSet()
	: BaseDamage(0.0f)
	, BaseHeal(0.0f)
	, BaseManaPerCast(0.0f)
{

	TagsToAttributes.Add(CoreGameplayTags::AttributesVitalMetaDamage, GetBaseDamageAttribute);
	TagsToAttributes.Add(CoreGameplayTags::AttributesVitalMetaHealing, GetBaseHealAttribute);
	
	SetIdentifierTag = CoreGameplayTags::AttributesVitalMeta;
}

void UDaCombatAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UDaCombatAttributeSet, BaseDamage, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDaCombatAttributeSet, BaseHeal, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDaCombatAttributeSet, BaseManaPerCast, COND_OwnerOnly, REPNOTIFY_Always);
}

void UDaCombatAttributeSet::OnRep_BaseDamage(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDaCombatAttributeSet, BaseDamage, OldValue);
}

void UDaCombatAttributeSet::OnRep_BaseHeal(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDaCombatAttributeSet, BaseHeal, OldValue);
}

void UDaCombatAttributeSet::OnRep_BaseManaPerCast(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDaCombatAttributeSet, BaseManaPerCast, OldValue);
}
