// Copyright Dream Awake Solutions LLC

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "DaAttributeData.generated.h"

USTRUCT(BlueprintType)
struct FDaAttributeData
{
	GENERATED_BODY()

	// Tag mapped to Attribute Variable in UAttributeSet derived subclass
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "Attributes"))
	FGameplayTag AttributeTag = FGameplayTag();

	// Localizable Display Name for Attribute
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeName = FText();

	// Localizable Description of Attribute
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeDescription = FText();

	UPROPERTY(BlueprintReadOnly)
	float AttributeValue = 0.f;
};

/**
 * 
 */
UCLASS()
class GAMEPLAYFRAMEWORK_API UDaAttributeInfo : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	FDaAttributeData FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound = false) const;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FDaAttributeData> AttributeInfo;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("AttributeInfoAssetId", GetFName());
	}
};
