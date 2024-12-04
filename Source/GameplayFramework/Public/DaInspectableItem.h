// Copyright Dream Awake Solutions LLC

#pragma once

#include "CoreMinimal.h"
#include "DaInteractableInterface.h"
#include "GameFramework/Actor.h"
#include "DaInspectableItem.generated.h"

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
	virtual void HighlightActor_Implementation() override;
	virtual void UnHighlightActor_Implementation() override;

	// Inspectable
	UFUNCTION(BlueprintCallable, Category = "Inspect")
	void Inspect(float ViewportPercentage = 0.6f, EInspectAlignment Alignment = EInspectAlignment::Center);

	UFUNCTION(BlueprintCallable, Category = "Inspect")
	void HideDetailedMesh();
	
protected:
	
	virtual void BeginPlay() override;

	// Preview and detailed mesh references
	UPROPERTY(EditAnywhere, Category = "Inspect")
	TSoftObjectPtr<UStaticMesh> PreviewMeshPtr;

	UPROPERTY(EditAnywhere, Category = "Inspect")
	TSoftObjectPtr<UStaticMesh> DetailedMeshPtr;

	// Preview location in world space
	UPROPERTY(EditAnywhere, Category = "Inspect")
	FVector PreviewLocation;

	// Viewport configuration
	UPROPERTY(EditAnywhere, Category = "Inspect")
	float ViewportPercentage = 0.6f;

	UPROPERTY(EditAnywhere, Category = "Inspect")
	float DistanceInFrontOfCamera = 200.0f;
	
	UPROPERTY(EditAnywhere, Category = "Inspect")
	EInspectAlignment InspectAlignment = EInspectAlignment::Center;

	UPROPERTY(EditAnywhere, Category = "Inspect")
	FName Name;
	
	UPROPERTY(BlueprintReadOnly)
	bool bHighlighted = false;

private:

	void OnDetailedMeshLoaded();
	void OnPreviewMeshLoaded();
	
	// Cached components
	UPROPERTY()
	UStaticMeshComponent* InspectMeshComponent;

	UPROPERTY()
	UStaticMeshComponent* PreviewMeshComponent;

	// Cached meshes
	UPROPERTY()
	UStaticMesh* LoadedPreviewMesh;

	UPROPERTY()
	UStaticMesh* LoadedDetailedMesh;

	// Asset loading handles
	TSharedPtr<FStreamableHandle> DetailedMeshHandle;
	TSharedPtr<FStreamableHandle> PreviewMeshHandle;

	// Current
	float CurrentViewportPercentage;
	EInspectAlignment CurrentAlignment;
};
