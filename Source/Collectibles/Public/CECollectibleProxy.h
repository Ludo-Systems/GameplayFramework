// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CECollectibleData.h"
#include "CECollectibleItemInterface.h"
#include "DaInspectableItem.h"
#include "CECollectibleProxy.generated.h"

struct FCECollectibleDataDef;

// Lightweight version of a collectible Object meant for UI items
UCLASS(BlueprintType)
class COLLECTIBLES_API UCECollectibleProxy : public UObject, public IDaInventoryItemInterface, public ICECollectibleItemInterface
{
	GENERATED_BODY()

public:
	static UCECollectibleProxy* CreateCollectibleProxyFromDataRef(const FCECollectibleDataDef* DataRef, const TSubclassOf<AActor>& ClassToSpawn);

	// IDaInventoryItemInterface
	virtual FName GetItemName_Implementation() const override { return CollectibleDataRef.CollectibleName; }
	virtual UTexture2D* GetItemThumbnail_Implementation() const override { return Thumbnail; }
	virtual FName GetItemDescription_Implementation() const override { return CollectibleDataRef.CollectibleName; }
	virtual int32 GetItemTags_Implementation(FGameplayTagContainer& OutItemTags) const override;
	virtual void AddToInventory_Implementation(APawn* InstigatorPawn, bool bDestroyActor = true) override;	

	// ICECollectibleItemInterface
	virtual FCECollectibleDataDef GetDataRef_Implementation() const override { return CollectibleDataRef; }

	UPROPERTY()
	FCECollectibleDataDef CollectibleDataRef;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collectible")
	UTexture2D* Thumbnail = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collectible")
	TSubclassOf<AActor> CollectibleActorClass = nullptr;
};