// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "DaWidgetMessageData.generated.h"

USTRUCT(BlueprintType)
struct FDaUIWidgetMessageData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag MessageTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText Message = FText();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UDaUserWidgetBase> MessageWidget;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTexture2D* Image = nullptr;

public:
	bool IsValid() const { return MessageTag.IsValid(); }
	
};

/**
 * 
 */
UCLASS()
class GAMEPLAYFRAMEWORK_API UDaWidgetMessageData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	FDaUIWidgetMessageData FindMessageDataForTag(const FGameplayTag& MessageTag, bool bLogNotFound = false) const;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FDaUIWidgetMessageData> MessageData;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("WidgetMessageData", GetFName());
	}
};
