// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/DaInventoryItemFactory.h"
#include "CEInventoryItemFactory.generated.h"

/**
 * 
 */
UCLASS()
class COLLECTIBLES_API UCEInventoryItemFactory : public UDaBaseInventoryItemFactory
{
	GENERATED_BODY()

public:
	virtual TSubclassOf<UDaInventoryItemBase> DetermineInventoryItemClassFromTags(const FGameplayTagContainer& Tags) const override;
};
