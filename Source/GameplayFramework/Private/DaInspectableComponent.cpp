// Fill out your copyright notice in the Description page of Project Settings.


#include "DaInspectableComponent.h"

#include "DaInspectableItem.h"
#include "GameplayFramework.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"

#define LOCTEXT_NAMESPACE "InspectableComponent"


UDaInspectableComponent::UDaInspectableComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

// TODO: Set above to true and implement tick to drive transform interactions for owning actor pawn from this component 
void UDaInspectableComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	APawn* MyPawn = CastChecked<APawn>(GetOwner());
	if(MyPawn->IsLocallyControlled())
	{
		// TODO: handle local user interactions with this inspectable Pawn
	}
}

bool UDaInspectableComponent::AreAssetsLoaded_Implementation()
{
	return DetailedMeshPtr.IsValid();
}

void UDaInspectableComponent::LoadAssets_Implementation()
{
	// Start async loading of the detailed mesh
	DetailedMeshHandle = UAssetManager::GetStreamableManager().RequestAsyncLoad(
		DetailedMeshPtr.ToSoftObjectPath(),
		FStreamableDelegate::CreateUObject(this, &UDaInspectableComponent::OnDetailedMeshLoaded)
	);
}

void UDaInspectableComponent::NativeOnAssetsLoaded()
{
	OnAssetsLoadedDelegate.Broadcast();
	Execute_OnAssetsLoaded(this);
}

void UDaInspectableComponent::OnDetailedMeshLoaded()
{
	if (DetailedMeshPtr.IsValid())
	{
		LoadedDetailedMesh = Cast<UStaticMesh>(DetailedMeshPtr.Get());
		if (LoadedDetailedMesh)
		{
			//DetailMeshComponent->SetStaticMesh(LoadedDetailedMesh);
			NativeOnAssetsLoaded();
		}
	}
	else
	{
		LOG_ERROR("Failed to load detailed mesh.");
	}
}

#undef LOCTEXT_NAMESPACE