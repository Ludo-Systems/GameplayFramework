// Copyright Dream Awake Solutions LLC

#pragma once

#include "CoreMinimal.h"
#include "DaInspectableInterface.h"
#include "DaInteractableInterface.h"
#include "GameFramework/Actor.h"
#include "Inventory/DaInventoryItemInterface.h"
#include "DaInspectableItem.generated.h"

class UDaInspectableComponent;
class USphereComponent;

UCLASS()
class GAMEPLAYFRAMEWORK_API ADaInspectableItem : public AActor , public IDaInspectableInterface
	, public IDaInteractableInterface, public IDaInventoryItemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADaInspectableItem();

	// IDaInspectableInterface
	virtual UStaticMeshComponent* GetPreviewMeshComponent_Implementation() const override;
	virtual UStaticMeshComponent* GetDetailedMeshComponent_Implementation() const override;
	virtual void OnInspectionStarted_Implementation(APawn* InstigatorPawn) override;
	virtual void OnInspectionEnded_Implementation(APawn* InstigatorPawn) override;
	
	// Interactable interface
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
	virtual void SecondaryInteract_Implementation(APawn* InstigatorPawn) override;
	virtual FText GetInteractText_Implementation(APawn* InstigatorPawn) override;

	// IDaInventoryItemInterface
	virtual FName GetItemName_Implementation() const override { return Name; }
	virtual FName GetItemDescription_Implementation() const override { return Description; }
	virtual int32 GetItemTags_Implementation(FGameplayTagContainer& OutItemTags) const override;
	virtual UMaterialInterface* GetRenderTargetMaterial_Implementation() const override { return RenderTargetMaterial; }
	virtual UDaAbilitySet* GetAbilitySet_Implementation() const override { return nullptr; }
	virtual UStaticMeshComponent* GetMeshComponent_Implementation() const override { return PreviewMeshComponent; }
	virtual void AddToInventory_Implementation(APawn* InstigatorPawn, bool bDestroyActor = true) override;
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<USphereComponent> SphereComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UStaticMeshComponent> PreviewMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> DetailedMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UDaInspectableComponent> InspectableComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	int CustomDepthStencilValue = 250;
	
	UPROPERTY(EditAnywhere, Category = "Interaction")
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	FText InteractionText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	FName Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	bool bCanBeAddedToInventory;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	FGameplayTagContainer TypeTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TObjectPtr<UMaterialInterface> RenderTargetMaterial;
};