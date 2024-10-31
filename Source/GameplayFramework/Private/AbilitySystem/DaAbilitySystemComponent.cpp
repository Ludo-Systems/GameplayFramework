// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/DaAbilitySystemComponent.h"

#include "AbilitySystem/DaAbilitySet.h"
#include "DaPawnData.h"

void UDaAbilitySystemComponent::InitAbilitiesWithPawnData(const UDaPawnData* DataAsset)
{
	if (DataAsset && GetOwner()->HasAuthority())
	{
		TArray<TObjectPtr<UDaAbilitySet>> AbilitySets = DataAsset->AbilitySets;
		for (auto AbilitySet : AbilitySets)
		{
			FDaAbilitySet_GrantedHandles Handle = FDaAbilitySet_GrantedHandles();
			OutGrantedAbilityHandlesArray.Add(Handle);
			AbilitySet->GiveToAbilitySystem(this, &Handle);
		}
	}
}

void UDaAbilitySystemComponent::GrantSet(const UDaAbilitySet* AbilitySet)
{
	if (AbilitySet && GetOwner()->HasAuthority())
	{
		FDaAbilitySet_GrantedHandles Handle = FDaAbilitySet_GrantedHandles();
		OutGrantedAbilityHandlesArray.Add(Handle);
		AbilitySet->GiveToAbilitySystem(this, &Handle);
	}
}

void UDaAbilitySystemComponent::ClearAbilitySets()
{
	if (GetOwner()->HasAuthority())
	{
		for (auto AbilitySetHandle: OutGrantedAbilityHandlesArray)
		{
			AbilitySetHandle.TakeFromAbilitySystem(this);
		}
	}
}

void UDaAbilitySystemComponent::AbilityInputTagPressed(const FInputActionValue& Value, const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;
	
	for (auto& AbilitySpec: GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (AbilitySpec.IsActive())
			{
				InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, AbilitySpec.Handle, AbilitySpec.ActivationInfo.GetActivationPredictionKey());
			}
		}
	}
}

void UDaAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag InputTag)
{
	if (!InputTag.IsValid()) return;

	for (auto& AbilitySpec: GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void UDaAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag InputTag)
{
	if (!InputTag.IsValid()) return;

	for (auto& AbilitySpec: GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)  && AbilitySpec.IsActive())
		{
			AbilitySpecInputReleased(AbilitySpec);
			InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, AbilitySpec.Handle, AbilitySpec.ActivationInfo.GetActivationPredictionKey());
		}
	}
}
