// Copyright Dream Awake Solutions LLC


#include "UI/DaWidgetController.h"

#include "AbilitySystem/DaAbilitySystemComponent.h"
#include "AbilitySystem/Attributes/DaAttributeData.h"
#include "AbilitySystem/Attributes/DaBaseAttributeSet.h"

void UDaWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSetTags = WCParams.AttributeSetTags;
}

void UDaWidgetController::BroadcastInitialValues()
{
	for (auto It = AttributeSetTags.CreateConstIterator(); It; ++It)
	{
		if (!It->IsValid()) continue;
		//LOG("Widget Controller Set Tag: %s", It->ToString());

		UDaBaseAttributeSet* AttributeSet = AbilitySystemComponent->GetAttributeSetForTag(*It);
		if (AttributeSet)
		{
			for (auto& Pair: AttributeSet->TagsToAttributes)
			{
				BroadcastAttributeInfo(Pair.Key, Pair.Value(), AttributeSet);
			}
		}
	}
}

void UDaWidgetController::BindCallbacksToDependencies()
{
	if (!IsValid(AbilitySystemComponent)) return;

	for (auto It = AttributeSetTags.CreateConstIterator(); It; ++It)
	{
		if (!It->IsValid()) continue;
		//LOG("Widget Controller Set Tag: %s", It->ToString());
		
		UDaBaseAttributeSet* AttributeSet = AbilitySystemComponent->GetAttributeSetForTag(*It);
		if (!IsValid(AttributeSet)) continue;
		
		for (auto& Pair: AttributeSet->TagsToAttributes)
		{
			// Store delegate handle for cleanup
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddWeakLambda(this,
				[this, Pair, AttributeSetPtr = TWeakObjectPtr<UDaBaseAttributeSet>(AttributeSet)](const FOnAttributeChangeData& Data)
			{
					if (!IsValid(this))
					{
						// We are using AddWeakLambda with this, so it should never be invalid at this point, but adding 
						LOG_WARNING("UDaWidgetController: Invalid Object");
						return;
					}
					
					if (!AttributeSetPtr.IsValid())
					{
						LOG_WARNING("UDaWidgetController: Invalid AttributeSet");
						return;
					}
					BroadcastAttributeInfo(Pair.Key, Pair.Value(), AttributeSetPtr.Get());
			});
		}
	}
}

void UDaWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag,
	const FGameplayAttribute& Attribute, UDaBaseAttributeSet* AttributeSet) const
{
	check(AttributeInfo);
	
	FDaAttributeData AttributeData = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	AttributeData.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(AttributeData);
}
