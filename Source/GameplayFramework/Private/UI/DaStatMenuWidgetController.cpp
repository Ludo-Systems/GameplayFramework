// Copyright Dream Awake Solutions LLC


#include "UI/DaStatMenuWidgetController.h"

#include "AbilitySystem/DaAbilitySystemComponent.h"
#include "AbilitySystem/Attributes/DaAttributeData.h"
#include "AbilitySystem/Attributes/DaBaseAttributeSet.h"

void UDaStatMenuWidgetController::BindCallbacksToDependencies()
{
	UDaBaseAttributeSet* AS = CastChecked<UDaBaseAttributeSet>(AttributeSet);
	check(AttributeInfo);

	for (auto& Pair: AS->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this, Pair](const FOnAttributeChangeData& Data)
		{
			BroadcastAttributeInfo(Pair.Key, Pair.Value());
		});
	}
}

void UDaStatMenuWidgetController::BroadcastInitialValues()
{
	UDaBaseAttributeSet* AS = CastChecked<UDaBaseAttributeSet>(AttributeSet);
	check(AttributeInfo);

	for (auto& Pair: AS->TagsToAttributes)
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
	}
}

void UDaStatMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag,
	const FGameplayAttribute& Attribute) const
{
	FDaAttributeData AttributeData = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	AttributeData.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(AttributeData);

}
