// Copyright Dream Awake Solutions LLC

#include "DaInspectableItem.h"

#include "GameplayFramework.h"
#include "Components/SphereComponent.h"
#include "Engine/AssetManager.h"

#define LOCTEXT_NAMESPACE "InspectableItems"

static TAutoConsoleVariable<bool> CVarInspectDebug(TEXT("da.InspectDebug"), false, TEXT("Log Inspectable Item Debug Info"), ECVF_Cheat);

ADaInspectableItem::ADaInspectableItem()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	RootComponent = SphereComp;
	
	PreviewMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PreviewMesh"));
	PreviewMeshComponent->SetupAttachment(RootComponent);
	PreviewMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	InspectMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("InspectMesh"));
	InspectMeshComponent->SetupAttachment(RootComponent);
	InspectMeshComponent->SetVisibility(false); // Initially hidden
	InspectMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	bReplicates = true;
}

void ADaInspectableItem::Inspect(APawn* InstigatorPawn, float ViewportPct,
                                 EInspectAlignment Alignment)
{
	// This is just for viewing on clients
	if (!InstigatorPawn->IsLocallyControlled()) return;

	if (bIsInspecting)
	{
		HideDetailedMesh();
		return;
	}
	
	// Hold a reference for tick
	InspectingPawn = InstigatorPawn;
	CurrentViewportPercentage = ViewportPct;
	CurrentAlignment = Alignment;
	bIsInspecting = true;
	InspectMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	if (LoadedDetailedMesh == nullptr)
	{
		LoadDetailMesh();
	}
	else
	{
		InspectMeshComponent->SetVisibility(true);
		PreviewMeshComponent->SetVisibility(false);
		
		PlaceDetailMeshInView();
	}
}

void ADaInspectableItem::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bIsInspecting && LoadedDetailedMesh)
	{
		PlaceDetailMeshInView();
	}
}

void ADaInspectableItem::LoadDetailMesh()
{
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
		if (LoadedDetailedMesh)
		{
			InspectMeshComponent->SetStaticMesh(LoadedDetailedMesh);
			
			InspectMeshComponent->SetVisibility(true);
			PreviewMeshComponent->SetVisibility(false);
			
			PlaceDetailMeshInView();
		}
	}
	else
	{
		LOG_ERROR("Failed to load detailed mesh.");
		HideDetailedMesh();
	}
}

// Adjust mesh placement in the viewport
void ADaInspectableItem::PlaceDetailMeshInView_Implementation()
{
	check(InspectingPawn);
	
	// Get mesh bounds
	FVector MeshOrigin, MeshExtent;
	LoadedDetailedMesh->GetBounds().GetBox().GetCenterAndExtents(MeshOrigin, MeshExtent);
	float MeshDiameter = 2.0f * MeshExtent.GetMax();
	
	// Calculate required scale to fit 60% of the viewport
	APlayerController* PlayerController = InspectingPawn->GetLocalViewingPlayerController();
	if (PlayerController && PlayerController->PlayerCameraManager)
	{
		FVector CameraLocation;
		FRotator CameraRotation;
		PlayerController->PlayerCameraManager->GetCameraViewPoint(CameraLocation, CameraRotation);
		
		float FOV = PlayerController->PlayerCameraManager->GetFOVAngle();
		float FOVHalfHeight = FMath::Tan(FMath::DegreesToRadians(FOV * 0.5f));

		// Dynamically adjust camera distance to fit the mesh
		float CameraDistance = (MeshDiameter / 2.0f) / FOVHalfHeight;

		// Add a multiplier to slightly increase the distance for a better fit
		CameraDistance *= CameraDistanceMultiplier; // Adjust multiplier as needed

		// Ensure a minimum distance to avoid clipping
		CameraDistance = FMath::Max(CameraDistance, MinCameraDistance);
		
		// Calculate the viewport height in world units using the dynamic CameraDistance
		float ViewportHeightWorldUnits = CameraDistance * FOVHalfHeight * 2.0f;
		
		// Calculate desired size in world units
		float DesiredSize = CurrentViewportPercentage * ViewportHeightWorldUnits;

		// Calculate the scaling factor to make the mesh fit the desired height
		float ScaleFactor = DesiredSize / MeshDiameter;
		
		// Position directly in front of the camera
		FVector ViewportPosition = CameraLocation + CameraRotation.Vector() * CameraDistance;

		// Scale
		InspectMeshComponent->SetWorldScale3D(FVector(ScaleFactor));
		
		// Update the component to ensure bounds are accurate
		InspectMeshComponent->MarkRenderStateDirty();
		InspectMeshComponent->UpdateBounds();
		
		// Get the updated mesh component's bounds in world space
		FBoxSphereBounds MeshBounds = InspectMeshComponent->Bounds;
		FVector MeshWorldCenter = MeshBounds.Origin;

		// Calculate the offset to center the mesh
		FVector CenteringOffset = ViewportPosition - MeshWorldCenter;

		FVector AlignmentOffset = FVector::ZeroVector;

		if (CurrentAlignment != EInspectAlignment::Center)
		{
			// Calculate viewport width for alignment offset 
			FVector2D ViewportSize;
			GEngine->GameViewport->GetViewportSize(ViewportSize);
			float AspectRatio = ViewportSize.X / ViewportSize.Y;
			float ViewportWidth = ViewportHeightWorldUnits * AspectRatio;
			
			switch (CurrentAlignment)
			{
			case EInspectAlignment::Left:
				AlignmentOffset = -CameraRotation.Quaternion().GetRightVector() * (ViewportWidth * AlignmentShiftMultiplier); // Left-shift
				break;
			case EInspectAlignment::Right:
				AlignmentOffset = CameraRotation.Quaternion().GetRightVector() * (ViewportWidth * AlignmentShiftMultiplier); // Right-shift
				break;
			default:
				break; // Centered
			}
		}
		
		InspectMeshComponent->SetWorldLocation(InspectMeshComponent->GetComponentLocation() + CenteringOffset + AlignmentOffset);

		// Align the mesh's rotation with the camera
		InspectMeshComponent->SetWorldRotation(CameraRotation);

		if (CVarInspectDebug.GetValueOnGameThread())
		{
			LOG("MeshOrigin: %s, MeshExtent: %s", *MeshOrigin.ToString(), *MeshExtent.ToString());
			LOG("MeshDiameter: %f, ViewportHeightWorldUnits: %f, DesiredHeight: %f, ScaleFactor: %f, FOV: %f",
				   MeshDiameter, ViewportHeightWorldUnits, DesiredSize, ScaleFactor, FOV);
			LOG("CameraDistance: %f, CenteringOffset: %s", CameraDistance, *CenteringOffset.ToString());
		}
	}
}

void ADaInspectableItem::HideDetailedMesh()
{
	bIsInspecting = false;
	
	// Hide the detailed mesh
	InspectMeshComponent->SetVisibility(false);
	PreviewMeshComponent->SetVisibility(true);

	// Reset Transform
	InspectMeshComponent->SetRelativeTransform(FTransform());

	InspectMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ADaInspectableItem::Interact_Implementation(APawn* InstigatorPawn)
{
	Inspect(InstigatorPawn, ViewportPercentage, InspectAlignment);
}

FText ADaInspectableItem::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	return FText::Format(LOCTEXT("Inspectable_HighlightText", "Inspect {0}"), FText::FromName(Name));
}

#undef LOCTEXT_NAMESPACE