// Copyright Dream Awake Solutions LLC

#pragma once

#include "AbilitySystemComponent.h"
#include "DaBaseAttributeSet.h"

#include "DaCombatAttributeSet.generated.h"

class UObject;
struct FFrame;

/**
 * 
 */
UCLASS()
class GAMEPLAYFRAMEWORK_API UDaCombatAttributeSet : public UDaBaseAttributeSet
{
	GENERATED_BODY()

public:

	UDaCombatAttributeSet();
	
	ATTRIBUTE_ACCESSORS(UDaCombatAttributeSet, BaseDamage);
	ATTRIBUTE_ACCESSORS(UDaCombatAttributeSet, BaseHeal);
	ATTRIBUTE_ACCESSORS(UDaCombatAttributeSet, BaseManaPerCast);

protected:

	UFUNCTION()
	void OnRep_BaseDamage(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_BaseHeal(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_BaseManaPerCast(const FGameplayAttributeData& OldValue);

private:

	// The base amount of damage to apply in the damage execution.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BaseDamage, Category = "DA|Combat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData BaseDamage;

	// The base amount of healing to apply in the heal execution.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BaseHeal, Category = "DA|Combat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData BaseHeal;

	// The base amount of mana to use when casting a spell or healing.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BaseManaPerCast, Category = "DA|Combat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData BaseManaPerCast;

};
