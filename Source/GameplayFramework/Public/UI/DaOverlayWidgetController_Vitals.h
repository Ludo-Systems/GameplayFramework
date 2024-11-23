// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DaOverlayWidgetController.h"
#include "DaOverlayWidgetController_Vitals.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);

/**
 * 
 */
UCLASS()
class GAMEPLAYFRAMEWORK_API UDaOverlayWidgetController_Vitals : public UDaOverlayWidgetController
{
	GENERATED_BODY()

public:
	
	UPROPERTY(BlueprintAssignable, Category="Character Attributes")
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category="Character Attributes")
	FOnAttributeChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Category="Character Attributes")
	FOnAttributeChangedSignature OnManaChanged;

	UPROPERTY(BlueprintAssignable, Category="Character Attributes")
	FOnAttributeChangedSignature OnMaxManaChanged;

protected:

	virtual void BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute, UDaBaseAttributeSet* AttributeSet) const override;

};
