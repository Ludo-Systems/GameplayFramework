// Fill out your copyright notice in the Description page of Project Settings.
// Fill out your copyright notice in the Description page of Project Settings.

#include "CECollectibleActorBase.h"

#include "DaCharacter.h"
#include "DaInspectableComponent.h"
#include "DaPlayerState.h"
#include "Inventory/DaInventoryComponent.h"

#define LOCTEXT_NAMESPACE "CollectibleItems"

// Sets default values
ACECollectibleActorBase::ACECollectibleActorBase()
{
	// Set this actor to call Tick() every frame
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	
	PreviewMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PreviewMesh"));
	PreviewMeshComp->SetupAttachment(RootComponent);
	PreviewMeshComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	DetailMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("DetailedMesh");
	DetailMeshComp->SetupAttachment(RootComponent);
	DetailMeshComp->SetVisibility(false);
	DetailMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	InspectableComp = CreateDefaultSubobject<UDaInspectableComponent>("InspectableComponent");
}

// Called when the game starts or when spawned
void ACECollectibleActorBase::BeginPlay()
{
	Super::BeginPlay();
}

// ICECollectibleItemInterface Implementation
int32 ACECollectibleActorBase::GetItemCoreTags_Implementation(FGameplayTagContainer& OutItemTags) const
{
	return CollectibleData->CollectibleDataRef.CoreTag.GetTagName().IsNone() ? 0 : OutItemTags.AddTag(CollectibleData->CollectibleDataRef.CoreTag);
}

USlateBrushAsset* ACECollectibleActorBase::GetItemBrush_Implementation() const
{
	// TODO: Implement getting item brush
	return nullptr;
}

UTexture2D* ACECollectibleActorBase::GetItemIcon_Implementation() const
{
	if (CollectibleData && CollectibleData->CoinCoreDataRef.Image.IsValid())
	{
		return CollectibleData->CoinCoreDataRef.Image.LoadSynchronous();
	}
	return nullptr;
}

FCECollectibleDataDef ACECollectibleActorBase::GetDataRef_Implementation() const
{
	return CollectibleData ? CollectibleData->CollectibleDataRef : FCECollectibleDataDef();
}

FCECoinCoreDataRef ACECollectibleActorBase::GetTemplateDataRef_Implementation() const
{
	return CollectibleData ? CollectibleData->CoinCoreDataRef : FCECoinCoreDataRef();
}

void ACECollectibleActorBase::SetCollectibleData_Implementation(UCECollectibleData* Data)
{
	CollectibleData = Data;
}

FString ACECollectibleActorBase::GetCollectibleDisplayNames_Implementation(bool bPlayerDesignated)
{
	// TODO: Implement getting collectible display names
	return FString();
}

void ACECollectibleActorBase::SaveCollectible_Implementation(bool bSaveToDisk)
{
	// TODO: Implement saving collectible
}

// IDaInventoryItemInterface Implementation
FName ACECollectibleActorBase::GetItemName_Implementation() const
{
	if (CollectibleData)
	{
		return CollectibleData->CollectibleDataRef.Name;
	}
	return FName();
}

FName ACECollectibleActorBase::GetItemDescription_Implementation() const
{
	if (CollectibleData)
	{
		return CollectibleData->CollectibleDataRef.Name;
	}
	return FName();
}

int32 ACECollectibleActorBase::GetItemTags_Implementation(FGameplayTagContainer& OutItemTags) const
{
	if (CollectibleData)
	{
		return CollectibleData->CollectibleDataRef.CoreTag.GetTagName().IsNone() ? 0 : OutItemTags.AddTag(CollectibleData->CollectibleDataRef.CoreTag);
	}
	return 0;
}

UTexture2D* ACECollectibleActorBase::GetItemThumbnail_Implementation() const
{
	if (CollectibleData && CollectibleData->CoinCoreDataRef.Image.IsValid())
	{
		return CollectibleData->CoinCoreDataRef.Image.LoadSynchronous();
	}
	return nullptr;
}

UMaterialInterface* ACECollectibleActorBase::GetRenderTargetMaterial_Implementation() const
{
	return RenderTargetMaterial;
}

UStaticMeshComponent* ACECollectibleActorBase::GetMeshComponent_Implementation() const
{
	return PreviewMeshComp;
}

UDaAbilitySet* ACECollectibleActorBase::GetAbilitySet_Implementation() const
{
	// TODO: Implement getting ability set
	return nullptr;
}

void ACECollectibleActorBase::AddToInventory_Implementation(APawn* InstigatorPawn, bool bDestroyActor)
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

// IDaInspectableInterface Implementation
void ACECollectibleActorBase::OnInspectionStarted_Implementation(APawn* InstigatorPawn)
{
	// TODO: Implement inspection start behavior
}

void ACECollectibleActorBase::OnInspectionEnded_Implementation(APawn* InstigatorPawn)
{
	// TODO: Implement inspection end behavior
}

UStaticMeshComponent* ACECollectibleActorBase::GetPreviewMeshComponent_Implementation() const
{
	return PreviewMeshComp;
}

UStaticMeshComponent* ACECollectibleActorBase::GetDetailedMeshComponent_Implementation() const
{
	return DetailMeshComp;
}

void ACECollectibleActorBase::Interact_Implementation(APawn* InstigatorPawn)
{
	if (InspectableComp == nullptr)
		return;
	
	if (InspectableComp->IsInspecting())
	{
		// If already inspecting, end inspection
		InspectableComp->HideDetailedMesh();
		return;
	}
	
	if (ADaCharacter* PlayerCharacter = Cast<ADaCharacter>(InstigatorPawn))
	{
		// Do this to give Gameplay Abilities access to the currently inspected item through the avatar or owning actor 
		PlayerCharacter->SetInspectedItem(this);
	}
	
	InspectableComp->Inspect(InstigatorPawn);
	
}

void ACECollectibleActorBase::SecondaryInteract_Implementation(APawn* InstigatorPawn)
{
	if (!InspectableComp->IsInspecting() && !bIsMoving)
	{
		Execute_AddToInventory(this, InstigatorPawn, true);
	}
}

FText ACECollectibleActorBase::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	return FText::Format(LOCTEXT("Collectible InteractText", "{0}"), FText::FromName(CollectibleData->CollectibleDataRef.Name));
}

#undef LOCTEXT_NAMESPACE
