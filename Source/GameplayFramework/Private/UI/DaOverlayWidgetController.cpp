// Copyright Dream Awake Solutions LLC


#include "UI/DaOverlayWidgetController.h"

#include "DaAttributeComponent.h"

void UDaOverlayWidgetController::BroadcastInitialValues()
{
	if (AttributeComponent)
	{
		OnHealthChanged.Broadcast(AttributeComponent->GetHealth());
		OnMaxHealthChanged.Broadcast(AttributeComponent->GetMaxHealth());

		OnManaChanged.Broadcast(AttributeComponent->GetMana());
		OnMaxManaChanged.Broadcast(AttributeComponent->GetMaxMana());
	}
}

void UDaOverlayWidgetController::BindCallbacksToDependencies()
{
	// Bind to our AttributeComponent Instead of directly using ASC->GetGameplayAttributeValueChangedDelegate(AttributeSet->GetHealthAttribute().AddUObject(this, ...))
	// It manages a lot more data derived from health and mana including death, damage, healing, shield buffs, etc...
	if (AttributeComponent)
	{
		AttributeComponent->OnHealthChanged.AddDynamic(this, &UDaOverlayWidgetController::HealthChanged);
		AttributeComponent->OnMaxHealthChanged.AddDynamic(this, &UDaOverlayWidgetController::MaxHealthChanged);
		AttributeComponent->OnManaChanged.AddDynamic(this, &UDaOverlayWidgetController::ManaChanged);
		AttributeComponent->OnMaxManaChanged.AddDynamic(this, &UDaOverlayWidgetController::MaxManaChanged);
	}
}

void UDaOverlayWidgetController::HealthChanged(UDaAttributeComponent* HealthComponent, float OldHealth, float NewHealth,
	AActor* InstigatorActor)
{
	OnHealthChanged.Broadcast(NewHealth);
}

void UDaOverlayWidgetController::MaxHealthChanged(UDaAttributeComponent* HealthComponent, float OldMaxHealth,
	float NewMaxHealth, AActor* InstigatorActor)
{
	OnMaxHealthChanged.Broadcast(NewMaxHealth);
}

void UDaOverlayWidgetController::ManaChanged(UDaAttributeComponent* HealthComponent, float OldMana, float NewMana,
	AActor* InstigatorActor)
{
	OnManaChanged.Broadcast(NewMana);
}

void UDaOverlayWidgetController::MaxManaChanged(UDaAttributeComponent* HealthComponent, float OldMaxMana,
	float NewMaxMana, AActor* InstigatorActor)
{
	OnMaxManaChanged.Broadcast(NewMaxMana);
}
