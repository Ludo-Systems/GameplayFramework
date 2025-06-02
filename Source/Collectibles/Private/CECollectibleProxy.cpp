// Fill out your copyright notice in the Description page of Project Settings.


#include "CECollectibleProxy.h"

#include "CECollectibleData.h"
#include "DaPlayerState.h"
#include "CollectiblesGameplayTags.h"
#include "Inventory/DaInventoryComponent.h"


UCECollectibleProxy* UCECollectibleProxy::CreateCollectibleProxyFromDataRef(const FCECollectibleDataDef* DataRef,
                                                                            const TSubclassOf<AActor>& ClassToSpawn)
{
	if (DataRef == nullptr)
	{
		return nullptr;
	}

	UCECollectibleProxy* CollectibleProxy = NewObject<UCECollectibleProxy>();
	CollectibleProxy->CollectibleDataRef = *DataRef;
	CollectibleProxy->CollectibleActorClass = ClassToSpawn;
	return CollectibleProxy;
}

int32 UCECollectibleProxy::GetItemTags_Implementation(FGameplayTagContainer& OutItemTags) const
{
	OutItemTags = FGameplayTagContainer(CollectiblesGameplayTags::TAG_InventoryTypeCollectibles);
	return 1;
}

void UCECollectibleProxy::AddToInventory_Implementation(APawn* InstigatorPawn, bool bDestroyActor)
{
	if (APlayerState* PS = InstigatorPawn->GetPlayerState())
	{
		if (UDaInventoryComponent* InventoryComponent = Cast<ADaPlayerState>(PS)->GetInventoryComponent())
		{
			if (InventoryComponent->AddItem(this))
			{
				Execute_ItemAddedToInventory(this);
			}
		}
	}
}
