// Copyright Dream Awake Solutions LLC


#include "DaItemChest.h"

#include "Net/UnrealNetwork.h"

#define LOCTEXT_NAMESPACE "InteractableActors"

// Sets default values
ADaItemChest::ADaItemChest()
{

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LidMesh"));
	LidMesh->SetupAttachment(BaseMesh);
	
	TargetPitch = 110;

	bReplicates = true;
}

void ADaItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	bLidOpened = !bLidOpened;
	OnRep_LidOpened();
}

void ADaItemChest::SecondaryInteract_Implementation(APawn* InstigatorPawn)
{
	// Derived classes to implement
}

FText ADaItemChest::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	if (bLidOpened)
	{
		return LOCTEXT("Chest_InteractMessage", "Close Chest");
	}
	else
	{
		return LOCTEXT("Chest_InteractMessage", "Open Chest");
	}
}

void ADaItemChest::HighlightActor_Implementation()
{
	bHighlighted = true;
	BaseMesh->SetRenderCustomDepth(true);
	BaseMesh->SetCustomDepthStencilValue(250.f);
	LidMesh->SetRenderCustomDepth(true);
	LidMesh->SetCustomDepthStencilValue(250.f);
}

void ADaItemChest::UnHighlightActor_Implementation()
{
	bHighlighted = false;
	BaseMesh->SetRenderCustomDepth(false);
	LidMesh->SetRenderCustomDepth(false);
}

void ADaItemChest::LoadActor_Implementation()
{
	OnRep_LidOpened();
}

bool ADaItemChest::ShouldLoadTransform_Implementation()
{
	return true;
}

void ADaItemChest::OnRep_LidOpened()
{
	float CurrentPitch = bLidOpened ? TargetPitch : 0.0f;
	LidMesh->SetRelativeRotation(FRotator(CurrentPitch, 0, 0));
	
	OnLidOpenedStateChanged.Broadcast(bLidOpened);
}

void ADaItemChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADaItemChest, bLidOpened);
}

#undef LOCTEXT_NAMESPACE