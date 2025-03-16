// Copyright Dream Awake Solutions LLC

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DaInteractableInterface.h"
#include "DaSaveInterface.h"
#include "DaItemChest.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLidOpenedStateChanged, bool, IsOpened);

UCLASS()
class GAMEPLAYFRAMEWORK_API ADaItemChest : public AActor, public IDaInteractableInterface, public IDaSaveInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADaItemChest();

	// IDaInteractableInterface
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
	virtual void SecondaryInteract_Implementation(APawn* InstigatorPawn) override;
	virtual FText GetInteractText_Implementation(APawn* InstigatorPawn) override;
	virtual void HighlightActor_Implementation() override;
	virtual void UnHighlightActor_Implementation() override;

	virtual void LoadActor_Implementation() override;
	virtual bool ShouldLoadTransform_Implementation() override;
	
	UPROPERTY(BlueprintAssignable, Category="ItemChest")
	FOnLidOpenedStateChanged OnLidOpenedStateChanged;
	
protected:

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> BaseMesh;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> LidMesh;
	
	UPROPERTY(EditAnywhere, Category="ItemChest")
	float TargetPitch;

	UPROPERTY(ReplicatedUsing="OnRep_LidOpened", BlueprintReadOnly, SaveGame)
	bool bLidOpened;

	UFUNCTION()
	void OnRep_LidOpened();

	UPROPERTY(BlueprintReadOnly)
	bool bHighlighted = false;
};
