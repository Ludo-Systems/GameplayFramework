// Copyright Dream Awake Solutions LLC


#include "AbilitySystem/DaAbilitySet.h"

#include "GameplayFramework.h"
#include "AbilitySystem/DaAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/DaGameplayAbilityBase.h"

void FDaAbilitySet_GrantedHandles::AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle)
{
	if (Handle.IsValid())
	{
		AbilitySpecHandles.Add(Handle);
	}
}

void FDaAbilitySet_GrantedHandles::AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle)
{
	if (Handle.IsValid())
	{
		GameplayEffectHandles.Add(Handle);
	}
}

void FDaAbilitySet_GrantedHandles::AddAttributeSet(UAttributeSet* Set)
{
	GrantedAttributeSets.Add(Set);
}

void FDaAbilitySet_GrantedHandles::TakeFromAbilitySystem(UDaAbilitySystemComponent* DaASC)
{
	check(DaASC);

	if (!DaASC->IsOwnerActorAuthoritative())
	{
		return;
	}

	for (const FGameplayAbilitySpecHandle& Handle : AbilitySpecHandles)
	{
		if (Handle.IsValid())
		{
			DaASC->ClearAbility(Handle);
		}
	}

	for (const FActiveGameplayEffectHandle& Handle : GameplayEffectHandles)
	{
		if (Handle.IsValid())
		{
			DaASC->RemoveActiveGameplayEffect(Handle);
		}
	}

	for (UAttributeSet* Set : GrantedAttributeSets)
	{
		DaASC->RemoveSpawnedAttribute(Set);
	}

	AbilitySpecHandles.Reset();
	GameplayEffectHandles.Reset();
	GrantedAttributeSets.Reset();
}

UDaAbilitySet::UDaAbilitySet(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UDaAbilitySet::GiveToAbilitySystem(UDaAbilitySystemComponent* DaASC,
	FDaAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject) const
{
	check(DaASC);

	if (!DaASC->IsOwnerActorAuthoritative())
	{
		return;
	}

	// Grant gameplay abilities
	for (int32 AbilityIndex = 0; AbilityIndex < GrantedGameplayAbilities.Num(); ++AbilityIndex)
	{
		const FDaAbilitySet_GameplayAbility& AbilityToGrant = GrantedGameplayAbilities[AbilityIndex];

		if (!IsValid(AbilityToGrant.Ability))
		{
			LOG_ERROR("GrantedGameplayAbilities[%d] on ability set [%s] is not valid.", AbilityIndex, *GetNameSafe(this));
			continue;
		}
		
		UDaGameplayAbilityBase* AbilityCDO = AbilityToGrant.Ability->GetDefaultObject<UDaGameplayAbilityBase>();
		LOG("GiveToAbilitySystem: %s", *GetNameSafe(AbilityCDO));

		FGameplayAbilitySpec AbilitySpec(AbilityCDO, AbilityToGrant.AbilityLevel);
		AbilitySpec.SourceObject = SourceObject;

		if (AbilityToGrant.InputTag.IsValid())
		{
			AbilitySpec.GetDynamicSpecSourceTags().AddTag(AbilityToGrant.InputTag);
		}
		
		const FGameplayAbilitySpecHandle AbilitySpecHandle = DaASC->GiveAbility(AbilitySpec);

		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddAbilitySpecHandle(AbilitySpecHandle);
		}
	}

	// Grant the gameplay effects.
	for (int32 EffectIndex = 0; EffectIndex < GrantedGameplayEffects.Num(); ++EffectIndex)
	{
		const FDaAbilitySet_GameplayEffect& EffectToGrant = GrantedGameplayEffects[EffectIndex];

		if (!IsValid(EffectToGrant.GameplayEffect))
		{
			LOG_ERROR("GrantedGameplayEffects[%d] on ability set [%s] is not valid", EffectIndex, *GetNameSafe(this));
			continue;
		}

		const UGameplayEffect* GameplayEffect = EffectToGrant.GameplayEffect->GetDefaultObject<UGameplayEffect>();
		const FActiveGameplayEffectHandle GameplayEffectHandle = DaASC->ApplyGameplayEffectToSelf(GameplayEffect, EffectToGrant.EffectLevel, DaASC->MakeEffectContext());

		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddGameplayEffectHandle(GameplayEffectHandle);
		}
	}

	// Grant the attribute sets.
	for (int32 SetIndex = 0; SetIndex < GrantedAttributes.Num(); ++SetIndex)
	{
		const FDaAbilitySet_AttributeSet& SetToGrant = GrantedAttributes[SetIndex];

		if (!IsValid(SetToGrant.AttributeSet))
		{
			LOG_ERROR("GrantedAttributes[%d] on ability set [%s] is not valid", SetIndex, *GetNameSafe(this));
			continue;
		}

		UAttributeSet* NewSet = NewObject<UAttributeSet>(DaASC->GetOwner(), SetToGrant.AttributeSet);
		DaASC->AddAttributeSetSubobject(NewSet);

		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddAttributeSet(NewSet);
		}
	}
	
}
