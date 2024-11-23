// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DaOverlayWidgetController_Vitals.h"

#include "AbilitySystem/Attributes/DaCharacterAttributeSet.h"

void UDaOverlayWidgetController_Vitals::BroadcastAttributeInfo(const FGameplayTag& AttributeTag,
                                                               const FGameplayAttribute& Attribute, UDaBaseAttributeSet* AttributeSet) const
{
	// Override this since this overlay class is health and mana specific and we dont want super class to broadcast generically
	// TODO: This is not really needed. Just need to make blueprints listen to the base class's delegate and act accordingly
	const float AttributeValue = Attribute.GetNumericValue(AttributeSet);
	const UDaCharacterAttributeSet* HealthSet = CastChecked<UDaCharacterAttributeSet>(AttributeSet);

	if (Attribute==HealthSet->GetHealthAttribute())
		OnHealthChanged.Broadcast(AttributeValue);

	if (Attribute==HealthSet->GetMaxHealthAttribute())
		OnMaxHealthChanged.Broadcast(AttributeValue);
	
	if (Attribute==HealthSet->GetManaAttribute())
		OnManaChanged.Broadcast(AttributeValue);

	if (Attribute==HealthSet->GetMaxHealthAttribute())
		OnMaxManaChanged.Broadcast(AttributeValue);
}
