// Copyright Dream Awake Solutions LLC


#include "DaPickup_Ability.h"

#include "AbilitySystem/DaAbilitySet.h"
#include "AbilitySystem/DaAbilitySystemComponent.h"

#define LOCTEXT_NAMESPACE "PickupAbilities"

void ADaPickup_Ability::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensureAlways(AbilitySetToGrant))
	{
		return;
	}

	UAbilitySystemComponent* PawnsASC = Cast<IAbilitySystemInterface>(InstigatorPawn)->GetAbilitySystemComponent();
	if (PawnsASC)
	{
		Cast<UDaAbilitySystemComponent>(PawnsASC)->GrantSet(AbilitySetToGrant);
	}

	HideAndCooldownItem(InstigatorPawn);
}

FText ADaPickup_Ability::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	if (!ensureAlways(AbilitySetToGrant))
	{
		return Super::GetInteractText_Implementation(InstigatorPawn);
	}
	
	constexpr int32 PlaceHolderCost = 20;
	return FText::Format(LOCTEXT("PickupAbility InteractText", "Receive {0} Ability. Cost {1} Credits."), FText::FromName(AbilitySetToGrant->GetSetIdentityTag().GetTagName()), PlaceHolderCost);
}


#undef LOCTEXT_NAMESPACE