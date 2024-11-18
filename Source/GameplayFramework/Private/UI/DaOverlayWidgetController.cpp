// Copyright Dream Awake Solutions LLC


#include "UI/DaOverlayWidgetController.h"

#include "DaAttributeComponent.h"
#include "AbilitySystem/DaAbilitySystemComponent.h"
#include "AbilitySystem/Attributes/DaCharacterAttributeSet.h"

void UDaOverlayWidgetController::BroadcastInitialValues()
{
	UDaCharacterAttributeSet* HealthSet = CastChecked<UDaCharacterAttributeSet>(AttributeSet);

	OnHealthChanged.Broadcast(HealthSet->GetHealth());
	OnMaxHealthChanged.Broadcast(HealthSet->GetMaxHealth());
	OnManaChanged.Broadcast(HealthSet->GetMana());
	OnMaxManaChanged.Broadcast(HealthSet->GetMaxMana());
}

void UDaOverlayWidgetController::BindCallbacksToDependencies()
{
	UDaCharacterAttributeSet* HealthSet = CastChecked<UDaCharacterAttributeSet>(AttributeSet);
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(HealthSet->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(HealthSet->GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		}
	);
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(HealthSet->GetManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnManaChanged.Broadcast(Data.NewValue);
		}
	);
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(HealthSet->GetMaxManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxManaChanged.Broadcast(Data.NewValue);
		}
	);
	
	Cast<UDaAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddLambda(
		[this](const FGameplayTagContainer& AssetTags)
	{
			for (const FGameplayTag& Tag: AssetTags)
			{
				FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(MessageParentTag);
				if (Tag.MatchesTag(MessageTag))
				{
					if (const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag))
					{
						MessageWidgetRowDelegate.Broadcast(*Row);
					}
				}
			}
	});
}
