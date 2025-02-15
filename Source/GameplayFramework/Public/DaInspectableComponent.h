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

	// Core inspection functionality
	UFUNCTION(BlueprintCallable, Category = "Inspect")
	void Inspect(APawn* InstigatorPawn, float ViewportPercentage = 0.5f, EInspectAlignment Alignment = EInspectAlignment::Center);

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
	float ViewportPercentage = 0.6f;

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
	EInspectAlignment InspectAlignment = EInspectAlignment::Center;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inspect")
	float AlignmentShiftMultiplier = 0.2f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inspect")
	bool bIsInspecting = false;

private:
	
	float CurrentViewportPercentage;
	EInspectAlignment CurrentAlignment;
	FTransform BaseDetailMeshTransform;

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

	// Cache for mesh components
	UPROPERTY(Transient, DuplicateTransient)
	TObjectPtr<APawn> InspectingPawn;

	UPROPERTY(Transient, DuplicateTransient)
	TObjectPtr<UStaticMeshComponent> PreviewMeshComponent;

	UPROPERTY(Transient, DuplicateTransient)
	TObjectPtr<UStaticMeshComponent> DetailedMeshComponent;
	
};
