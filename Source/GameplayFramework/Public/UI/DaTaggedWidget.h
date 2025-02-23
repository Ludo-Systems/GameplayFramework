// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "GameplayEffectTypes.h"
#include "DaTaggedWidget.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class GAMEPLAYFRAMEWORK_API UDaTaggedWidget : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	UDaTaggedWidget(const FObjectInitializer& ObjectInitializer);

	//~UUserWidget interface
	virtual void NativeConstruct() override;
	//~End of UUserWidget interface

protected:

	UPROPERTY(EditDefaultsOnly, Category = "GameplayTags")
	FGameplayTagBlueprintPropertyMap GameplayTagPropertyMap;
	
};
