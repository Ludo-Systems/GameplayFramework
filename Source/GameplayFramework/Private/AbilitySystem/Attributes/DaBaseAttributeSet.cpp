// Copyright Dream Awake Solutions LLC


#include "AbilitySystem/Attributes/DaBaseAttributeSet.h"

#include "AbilitySystem/DaAbilitySystemComponent.h"

class UWorld;

UDaBaseAttributeSet::UDaBaseAttributeSet()
{
}

UWorld* UDaBaseAttributeSet::GetWorld() const
{
	const UObject* Outer = GetOuter();
	check(Outer);

	return Outer->GetWorld();
}

UDaAbilitySystemComponent* UDaBaseAttributeSet::GetDaAbilitySystemComponent() const
{
	return Cast<UDaAbilitySystemComponent>(GetOwningAbilitySystemComponent());
}
