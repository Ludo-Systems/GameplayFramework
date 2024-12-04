// Copyright Dream Awake Solutions LLC

#include "DaInspectableItem.h"

#include "GameplayFramework.h"
#include "Engine/AssetManager.h"

#define LOCTEXT_NAMESPACE "InspectableItems"

ADaInspectableItem::ADaInspectableItem()
{
	PrimaryActorTick.bCanEverTick = false;

	// Initialize the preview mesh component
	PreviewMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PreviewMesh"));
	PreviewMeshComponent->SetupAttachment(RootComponent);

	// Initialize the detailed mesh component
	InspectMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("InspectMesh"));
	InspectMeshComponent->SetupAttachment(RootComponent);
	InspectMeshComponent->SetVisibility(false); // Initially hidden

	// Set default preview location
	PreviewLocation = FVector::ZeroVector;
}

void ADaInspectableItem::BeginPlay()
{
	Super::BeginPlay();

	// Store initial transform to return item to initial location if moved
	HideDetailedMesh();
}

void ADaInspectableItem::Inspect(float ViewportPct,
                                 EInspectAlignment Alignment)
{
	if (!DetailedMeshPtr.IsValid())
	{
		LOG_WARNING("Invalid MeshPtr passed to Inspect.");
		return;
	}

	CurrentViewportPercentage = ViewportPct;
	CurrentAlignment = Alignment;

	// Start async loading of the detailed mesh
	DetailedMeshHandle = UAssetManager::GetStreamableManager().RequestAsyncLoad(
		DetailedMeshPtr.ToSoftObjectPath(),
		FStreamableDelegate::CreateUObject(this, &ADaInspectableItem::OnDetailedMeshLoaded)
	);
}

void ADaInspectableItem::OnDetailedMeshLoaded()
{
	if (DetailedMeshPtr.IsValid())
	{
		LoadedDetailedMesh = Cast<UStaticMesh>(DetailedMeshPtr.Get());
		if (LoadedDetailedMesh && InspectMeshComponent)
		{
			InspectMeshComponent->SetStaticMesh(LoadedDetailedMesh);
			InspectMeshComponent->SetVisibility(true);

			// Hide the preview mesh
			if (PreviewMeshComponent)
			{
				PreviewMeshComponent->SetVisibility(false);
			}

			// Adjust mesh placement in the viewport
			
			// Get mesh bounds
			FVector MeshOrigin, MeshBoundsExtent;
			LoadedDetailedMesh->GetBounds().GetBox().GetCenterAndExtents(MeshOrigin, MeshBoundsExtent);

			// Calculate required scale to fit 60% of the viewport
			APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
			if (PlayerController && PlayerController->PlayerCameraManager)
			{
				FVector CameraLocation;
				FRotator CameraRotation;
				PlayerController->PlayerCameraManager->GetCameraViewPoint(CameraLocation, CameraRotation);
	
				float FOV = PlayerController->PlayerCameraManager->GetFOVAngle();
				FVector2D ViewportSize;
				GEngine->GameViewport->GetViewportSize(ViewportSize);
	
				float AspectRatio = ViewportSize.X / ViewportSize.Y;
				float ViewportHeight = 2.0f * FMath::Tan(FMath::DegreesToRadians(FOV * 0.5f));
				float ViewportWidth = ViewportHeight * AspectRatio;
	
				// Calculate scaling factor
				float MeshDiameter = 2.0f * MeshBoundsExtent.GetMax();
				float DesiredSize = CurrentViewportPercentage * ViewportHeight;
				float ScaleFactor = DesiredSize / MeshDiameter;
	
				// Set position and scale
				FVector Offset = CameraRotation.Vector() * 200.0f; // Distance in front of the camera
				FVector AlignmentOffset = FVector::ZeroVector;
	
				switch (CurrentAlignment)
				{
				case EInspectAlignment::Left:
					AlignmentOffset = -CameraRotation.Quaternion().GetRightVector() * (ViewportWidth * 0.2f); // Left-shift
					break;
				case EInspectAlignment::Right:
					AlignmentOffset = CameraRotation.Quaternion().GetRightVector() * (ViewportWidth * 0.2f); // Right-shift
					break;
				default:
					break; // Centered
				}
	
				InspectMeshComponent->SetWorldLocation(CameraLocation + Offset + AlignmentOffset);
				InspectMeshComponent->SetWorldRotation(CameraRotation);
				InspectMeshComponent->SetWorldScale3D(FVector(ScaleFactor));
			}
		}
	}
	else
	{
		LOG_ERROR("Failed to load detailed mesh.");
	}
}


void ADaInspectableItem::HideDetailedMesh()
{
	// Hide the detailed mesh
	if (InspectMeshComponent)
	{
		InspectMeshComponent->SetVisibility(false);
	}

	// Start async loading of the preview mesh if not already loaded
	if (!LoadedPreviewMesh && PreviewMeshPtr.IsValid())
	{
		PreviewMeshHandle = UAssetManager::GetStreamableManager().RequestAsyncLoad(
			PreviewMeshPtr.ToSoftObjectPath(),
			FStreamableDelegate::CreateUObject(this, &ADaInspectableItem::OnPreviewMeshLoaded)
		);
	}
	else if (LoadedPreviewMesh)
	{
		// Show the preview mesh if already loaded
		if (PreviewMeshComponent)
		{
			PreviewMeshComponent->SetStaticMesh(LoadedPreviewMesh);
			PreviewMeshComponent->SetVisibility(true);
			PreviewMeshComponent->SetWorldLocation(PreviewLocation);
		}
	}
}

void ADaInspectableItem::OnPreviewMeshLoaded()
{
	if (PreviewMeshPtr.IsValid())
	{
		LoadedPreviewMesh = Cast<UStaticMesh>(PreviewMeshPtr.Get());
		if (LoadedPreviewMesh && PreviewMeshComponent)
		{
			PreviewMeshComponent->SetStaticMesh(LoadedPreviewMesh);
			PreviewMeshComponent->SetVisibility(true);
			PreviewMeshComponent->SetWorldLocation(PreviewLocation);
		}
	}
	else
	{
		LOG_ERROR("Failed to load preview mesh.");
	}
}


void ADaInspectableItem::Interact_Implementation(APawn* InstigatorPawn)
{
	Inspect(ViewportPercentage, InspectAlignment);
}

FText ADaInspectableItem::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	return FText::Format(LOCTEXT("Inspectable_HighlightText", "Inspect {0}"), FText::FromName(Name));
}

void ADaInspectableItem::HighlightActor_Implementation()
{
	bHighlighted = true;
}

void ADaInspectableItem::UnHighlightActor_Implementation()
{
	bHighlighted = false;
}

#undef LOCTEXT_NAMESPACE