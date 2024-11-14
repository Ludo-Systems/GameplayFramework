// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Modifiers/DaMMC_Health.h"

#include "AbilitySystem/Attributes/DaCharacterAttributeSet.h"

UDaMMC_Health::UDaMMC_Health()
{
	HealthDef.AttributeToCapture = UDaCharacterAttributeSet::GetHealthAttribute();
	HealthDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	HealthDef.bSnapshot = false; 
	
	MaxHealthDef.AttributeToCapture = UDaCharacterAttributeSet::GetMaxHealthAttribute();
	MaxHealthDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	MaxHealthDef.bSnapshot = false; 

	RelevantAttributesToCapture.Add(HealthDef);
	RelevantAttributesToCapture.Add(MaxHealthDef);
}

float UDaMMC_Health::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;
	
	float Health = 0.0f;
	GetCapturedAttributeMagnitude(HealthDef, Spec, EvaluateParameters, Health);
	
	float MaxHealth = 0.0f;
	GetCapturedAttributeMagnitude(MaxHealthDef, Spec, EvaluateParameters, MaxHealth);

	// if (Health+1.0f < MaxHealth)
	// {
	// 	return MaxHealth - Health;
	// }
	//return 1.0f;

	// return 0 if we're at max
	return FMath::Clamp(MaxHealth-Health, 0.0f, 1.0f);
}
