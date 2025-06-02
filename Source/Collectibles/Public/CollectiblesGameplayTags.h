// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NativeGameplayTags.h"

// Use our own macro (which adds the module name) to declare the gameplay tags 
#define CE_DECLARE_GAMEPLAY_TAG_EXTERN(TagName) COLLECTIBLES_API extern FNativeGameplayTag TagName;

namespace CollectiblesGameplayTags
{
	// Collectibles
	CE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Collectibles);

	// Core Tags used to define the Collectible Type
	CE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_CollectiblesCore);
	CE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_CollectiblesCoreCoin);
	CE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_CollectiblesCoreCard);
	CE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_CollectiblesCoreStamp);
	CE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_CollectiblesCoreRecord);

	// Special Collectible Tags
	CE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_CollectiblesSpecial);
	CE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_CollectiblesSpecialError);

	// Inventory System
	CE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_InventoryTypeCollectibles);
	
	// Coin Attributes
	CE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_AttributesStatsCollectible);
	CE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_AttributesStatsPrimaryCollectible_AppraisedGrade);
	CE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_AttributesStatsSecondaryCollectible_DerivedValue);

}
