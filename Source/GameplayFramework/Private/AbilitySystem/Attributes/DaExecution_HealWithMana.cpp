// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Attributes/DaExecution_HealWithMana.h"

#include "GameplayFramework.h"
#include "AbilitySystem/Attributes/DaCharacterAttributeSet.h"
#include "AbilitySystem/Attributes/DaCombatAttributeSet.h"

struct FHealingStatics
{
	FGameplayEffectAttributeCaptureDefinition BaseHealingRef;
	FGameplayEffectAttributeCaptureDefinition BaseManaPerCastRef;

	FHealingStatics()
	{
		BaseHealingRef = FGameplayEffectAttributeCaptureDefinition(UDaCombatAttributeSet::GetBaseHealAttribute(), EGameplayEffectAttributeCaptureSource::Source, true);
		BaseManaPerCastRef = FGameplayEffectAttributeCaptureDefinition(UDaCombatAttributeSet::GetBaseManaPerCastAttribute(), EGameplayEffectAttributeCaptureSource::Source, true);
	}
};

static FHealingStatics& HealingStatics()
{
	static FHealingStatics HealingStaticsRef;
	return HealingStaticsRef;
}

UDaExecution_HealWithMana::UDaExecution_HealWithMana()
{
	RelevantAttributesToCapture.Add(HealingStatics().BaseHealingRef);
	RelevantAttributesToCapture.Add(HealingStatics().BaseManaPerCastRef);
}

void UDaExecution_HealWithMana::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                               FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	
#if WITH_SERVER_CODE
	
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParams;
	EvaluationParams.SourceTags = SourceTags;
	EvaluationParams.TargetTags = TargetTags;

	float Mana = 0.0f;
	UAbilitySystemComponent* SourceASC = ExecutionParams.GetTargetAbilitySystemComponent();
	if (SourceASC)
	{
		Mana = SourceASC->GetSet<UDaCharacterAttributeSet>()->GetMana();
	}
	
	float BaseHeal = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(HealingStatics().BaseHealingRef, EvaluationParams, BaseHeal);
	float HealingAmount = BaseHeal;

	float BaseManaPerCast = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(HealingStatics().BaseManaPerCastRef, EvaluationParams, BaseManaPerCast);

	LOG("Heal Execution: Mana: %f, Healing Amount: %f, BaseManaPerCast: %f", Mana, HealingAmount, BaseManaPerCast);
	
	if (HealingAmount > 0.0f)
	{
		if (Mana >= BaseManaPerCast)
		{
			OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UDaCharacterAttributeSet::GetHealingAttribute(), EGameplayModOp::Additive, HealingAmount));
			OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UDaCharacterAttributeSet::GetManaAttribute(), EGameplayModOp::Additive, -BaseManaPerCast));
		}
	}

#endif // #if WITH_SERVER_CODE

}
