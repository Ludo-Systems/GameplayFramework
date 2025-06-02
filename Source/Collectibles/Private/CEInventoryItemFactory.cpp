// Fill out your copyright notice in the Description page of Project Settings.


#include "CEInventoryItemFactory.h"

#include "CEInventoryItem.h"
#include "CollectiblesGameplayTags.h"
#include "Inventory/DaInventoryItemInterface.h"

TSubclassOf<UDaInventoryItemBase> UCEInventoryItemFactory::DetermineInventoryItemClassFromTags(
	const FGameplayTagContainer& Tags) const
{
	if (Tags.IsValid())
	{
		if (Tags.HasTagExact(CollectiblesGameplayTags::TAG_InventoryTypeCollectibles))
		{
			return UCEInventoryItem::StaticClass();
		}
	}
	
	return Super::DetermineInventoryItemClassFromTags(Tags);
}