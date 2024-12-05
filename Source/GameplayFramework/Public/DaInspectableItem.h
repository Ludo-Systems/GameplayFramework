// Copyright Dream Awake Solutions LLC

#pragma once

#include "CoreMinimal.h"
#include "DaInteractableInterface.h"
#include "GameFramework/Actor.h"
#include "DaInspectableItem.generated.h"

class USphereComponent;
struct FStreamableHandle;

UENUM(BlueprintType)
enum class EInspectAlignment : uint8
{
	Center UMETA(DisplayName = "Center"),
	Left UMETA(DisplayName = "Left"),
	Right UMETA(DisplayName = "Right")
};

UCLASS()
class GAMEPLAYFRAMEWORK_API ADaInspectableItem : public AActor , public IDaInteractableInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADaInspectableItem();

	// Interactable interface
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
	virtual FText GetInteractText_Implementation(APawn* InstigatorPawn) override;

	// Inspectable
	UFUNCTION(BlueprintCallable, Category = "Inspect")
	void Inspect(APawn* InstigatorPawn, float ViewportPercentage = 0.5f, EInspectAlignment Alignment = EInspectAlignment::Center);

	UFUNCTION(BlueprintCallable, Category = "Inspect")
	void HideDetailedMesh();
	
protected:
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Components")
	TObjectPtr<USphereComponent> SphereComp;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Components")
	TObjectPtr<UStaticMeshComponent> PreviewMeshComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Components")
	TObjectPtr<UStaticMeshComponent> InspectMeshComponent;

	UPROPERTY(EditAnywhere, Category = "Inspect")
	TSoftObjectPtr<UStaticMesh> DetailedMeshPtr;

	// Viewport configuration
	UPROPERTY(EditAnywhere, Category = "Inspect")
	float ViewportPercentage = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Inspect")
	float CameraDistanceMultiplier = 2.5f;

	UPROPERTY(EditAnywhere, Category = "Inspect")
	float MinCameraDistance = 300.0f;
	
	UPROPERTY(EditAnywhere, Category = "Inspect")
	EInspectAlignment InspectAlignment = EInspectAlignment::Center;

	UPROPERTY(EditAnywhere, Category = "Inspect")
	float AlignmentShiftMultiplier = 0.2f;
	
	UPROPERTY(EditAnywhere, Category = "Inspect")
	FName Name;

	UFUNCTION(BlueprintNativeEvent, Category="Inspect")
	void PlaceDetailMeshInView();

	virtual void Tick(float DeltaSeconds) override;

private:

	UPROPERTY()
	UStaticMesh* LoadedDetailedMesh;
	
	void LoadDetailMesh();
	void OnDetailedMeshLoaded();
	
	// Asset loading handles
	TSharedPtr<FStreamableHandle> DetailedMeshHandle;

	// Current
	float CurrentViewportPercentage;
	EInspectAlignment CurrentAlignment;

	bool bIsInspecting = false;

	UPROPERTY()
	APawn* InspectingPawn;
	
};
