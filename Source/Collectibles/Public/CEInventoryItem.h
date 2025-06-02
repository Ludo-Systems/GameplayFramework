// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CECollectibleItemInterface.h"
#include "Inventory/DaStackableInventoryItem.h"
#include "CEInventoryItem.generated.h"

class ACECollectibleBase;
/**
 * 
 */
UCLASS()
class COLLECTIBLES_API UCEInventoryItem : public UDaStackableInventoryItem, public ICECollectibleItemInterface
{
	GENERATED_BODY()

public:

	virtual int32 GetItemCoreTags_Implementation(FGameplayTagContainer& OutItemTags) const override;
	virtual USlateBrushAsset* GetItemBrush_Implementation() const override;
	virtual FCECollectibleDataDef GetDataRef_Implementation() const override;

	UFUNCTION(BlueprintCallable, Category="Collectibles")
	UObject* GetCollectibleObject() const;
	
};
