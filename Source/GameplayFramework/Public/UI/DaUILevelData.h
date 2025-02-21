// Copyright Dream Awake Solutions LLC

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "DaUILevelData.generated.h"

class UDaWidgetController;
class UDaUserWidgetBase;
/**
 * 
 */
UCLASS(BlueprintType, Const)
class GAMEPLAYFRAMEWORK_API UDaUILevelData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	// Maps to Level name in Unreal
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=UI)
	TSoftObjectPtr<UWorld> Level;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=UI)
	TSubclassOf<UDaUserWidgetBase> WidgetClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=UI)
	TSubclassOf<UDaWidgetController> WidgetControllerClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=UI)
	FGameplayTagContainer WidgetGameplayAttributeSetTags;
	
	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("UILevelData", GetFName());
	}
};
