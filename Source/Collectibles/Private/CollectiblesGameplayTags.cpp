// Fill out your copyright notice in the Description page of Project Settings.


#include "CollectiblesGameplayTags.h"

namespace CollectiblesGameplayTags
{
	// Collectibles
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Collectibles, "Collectibles", "Collectibles Parent Tag. Child tags should follow the format: Collectibles.<Core or Special>.<Specifics>");

	// Core Tags used to define the Collectible Type
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_CollectiblesCore, "Collectibles.Core", "Root Parent for Core Collectible Types, subtypes, Edition. Child Tags should follow the format: Collectibles.Core.<type>.<subtype>.<edition>");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_CollectiblesCoreCoin, "Collectibles.Core.Coin", "Collectibles Core Parent Tag. Child Tags should follow the format: Collectibles.Coin.<subtype>.<denomination>");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_CollectiblesCoreCard, "Collectibles.Core.Card", "Card Collectibles Type. Child Tags should follow the format: Collectibles.Card.<subtype>.<denomination>");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_CollectiblesCoreStamp, "Collectibles.Core.Stamp", "Stamp Collectibles Type. Child Tags should follow the format: Collectibles.Card.<subtype>.<denomination>");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_CollectiblesCoreRecord, "Collectibles.Core.Record", "Record Collectibles Type. Child Tags should follow the format: Collectibles.Coin.<subtype>.<denomination>");

	// Special Tags for Collectibles used to define things like errors or mintmarks
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_CollectiblesSpecial, "Collectibles.Special", "Special Optional Tags for Collectibles denoting specific common features as GameplayTags. Child tags should follow the format: Collectibles.Special.<SpecialData>");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_CollectiblesSpecialError, "Collectibles.Special.Error", "Error on the Collectible. This tag suggests the Collectible instance will have one or more Strings specifying the specific error.");

	// Special Inventory Types for the inventory system to know how to handle Collectibles
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_InventoryTypeCollectibles, "Inventory.Type.Collectibles", "Inventory Item Type Tag for Collectibles, required to create specific subclass needed for a collectible inventory item");
	
	// Collectible Attributes
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_AttributesStatsCollectible, "Attributes.Stats.Collectible", "Set identifier for Collectible Stats Set");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_AttributesStatsPrimaryCollectible_AppraisedGrade, "Attributes.Stats.Primary.Collectible.AppraisedGrade", "Appraised grade of coin set by player");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_AttributesStatsSecondaryCollectible_DerivedValue, "Attributes.Stats.Secondary.Collectible.DerivedValue", "Derived value of coin based off various other factors.");
}
