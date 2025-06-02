// Fill out your copyright notice in the Description page of Project Settings.


#include "Collectibles/Public/CEInventoryItem.h"
#include "Collectibles/Public/CECollectibleProxy.h"

int32 UCEInventoryItem::GetItemCoreTags_Implementation(FGameplayTagContainer& OutItemTags) const
{
	OutItemTags = InventoryItemTags;
	return OutItemTags.Num();
}

USlateBrushAsset* UCEInventoryItem::GetItemBrush_Implementation() const
{
	return ThumbnailBrush;
}

FCECollectibleDataDef UCEInventoryItem::GetDataRef_Implementation() const
{
	if (UObject* BaseObj = GetCollectibleObject())
	{
		if (BaseObj->GetClass()->ImplementsInterface(UCECollectibleItemInterface::StaticClass()))
		{
			return ICECollectibleItemInterface::Execute_GetDataRef(BaseObj);
		}
	}
	
	return FCECollectibleDataDef();
}

UObject* UCEInventoryItem::GetCollectibleObject() const
{
	if (BaseObject.IsValid())
	{
		return BaseObject.Get();
	}
	return nullptr;
}
