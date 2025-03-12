// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DaInspectableComponent.generated.h"

UENUM(BlueprintType)
enum class EInspectAlignment : uint8
{
	Center UMETA(DisplayName = "Center"),
	Left UMETA(DisplayName = "Left"),
	Right UMETA(DisplayName = "Right")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnInspectStateChanged,  AActor*, InspectedItem, AActor*, InspectingActor, bool, IsInspecting);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMEPLAYFRAMEWORK_API UDaInspectableComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDaInspectableComponent();

	/** Begins inspection using current settings. Will use default settings if ConfigureInspection() 
	 * wasn't called first.
	 * @param InstigatorPawn The pawn that initiated the inspection
	 */
	UFUNCTION(BlueprintCallable, Category = "Inspect", 
		meta=(DisplayName="Start Inspection",
			Tooltip="Begins inspection using current or default settings"))
	void Inspect(APawn* InstigatorPawn);

	// Optional configuration function - call before Inspect() to override defaults
	UFUNCTION(BlueprintCallable, Category = "Inspect", 
		meta=(DisplayName="Configure Inspection Settings"))
	void ConfigureInspection(float NewViewportPercentage, EInspectAlignment NewAlignment);

	// Runtime adjustment
	UFUNCTION(BlueprintCallable, Category = "Inspect|Runtime Adjustment",
		meta=(DisplayName="Change Inspection Alignment"))
	void SetAlignment(EInspectAlignment NewAlignment);
	
	UFUNCTION(BlueprintCallable, Category = "Inspect")
	void HideDetailedMesh();

	UFUNCTION(BlueprintCallable, Category = "Inspect")
	void RotateDetailedMesh(float DeltaPitch, float DeltaYaw);

	UFUNCTION(BlueprintCallable, Category = "Inspect")
	void ZoomDetailedMesh(float DeltaZoom);
	
	UPROPERTY(BlueprintAssignable, Category="Inspect")
	FOnInspectStateChanged OnInspectStateChanged;
	
	FBoxSphereBounds GetHierarchyBounds(USceneComponent* RootComponent, bool bMeshLocalSpace);
	
protected:
	
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inspect")
	bool bShowDetailAsPreview = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inspect")
	EInspectAlignment DefaultAlignment = EInspectAlignment::Center;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inspect")
	float DefaultViewportPercentage = 0.6f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inspect")
	float CameraDistanceMultiplier = 1.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inspect")
	float MinCameraDistance = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inspect")
	float MaxCameraDistance = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inspect")
	float InitialCameraDistanceOffset = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inspect")
	float ZoomSmoothingFactor = 0.9f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inspect")
	float RotationSmoothingSpeed = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inspect")
	float AlignmentShiftMultiplier = 0.2f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inspect")
	bool bIsInspecting = false;

private:
	
	float CurrentViewportPercentage;
	EInspectAlignment CurrentAlignment;
	FTransform BaseDetailMeshTransform;
	FVector AlignmentOffset = FVector::ZeroVector;

	// Mesh properties
	float CameraDistance;
	FRotator CurrentRotation;
	FVector CurrentLocation;

	// Input deltas
	float InputDeltaPitch;
	float InputDeltaYaw;
	float InputDeltaZoom;

	// Bounds and scaling
	float ScaleFactor;
	FVector CenteringOffset;

	// Utility functions from original class
	void UpdateMeshTransform(float DeltaTime);
	void PlaceDetailMeshInView();
	FVector CalculateAlignmentOffset(const FRotator& CameraRotation, float CurrentCameraDistance);

	// Cache for mesh components
	UPROPERTY(Transient, DuplicateTransient)
	TObjectPtr<APawn> InspectingPawn;

	UPROPERTY(Transient, DuplicateTransient)
	TObjectPtr<UStaticMeshComponent> PreviewMeshComponent;

	UPROPERTY(Transient, DuplicateTransient)
	TObjectPtr<UStaticMeshComponent> DetailedMeshComponent;
	
};
