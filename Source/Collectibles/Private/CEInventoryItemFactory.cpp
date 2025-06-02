// Fill out your copyright notice in the Description page of Project Settings.


#include "Collectibles/Public/CEInventoryItemFactory.h"

#include "Collectibles/Public/CEInventoryItem.h"
#include "../../../../../Source/CollectorsEdition/CEGameplayTags.h"
#include "Inventory/DaInventoryItemInterface.h"

TSubclassOf<UDaInventoryItemBase> UCEInventoryItemFactory::DetermineInventoryItemClassFromTags(
	const FGameplayTagContainer& Tags) const
{
	if (Tags.IsValid())
	{
		if (Tags.HasTagExact(CEGameplayTags::TAG_InventoryTypeCollectibles))
		{
			return UCEInventoryItem::StaticClass();
		}
	}
	
	return Super::DetermineInventoryItemClassFromTags(Tags);
}