// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DaInspectableInterface.h"
#include "GameFramework/Actor.h"
#include "DaInspectableComponent.generated.h"

struct FStreamableHandle;

UCLASS(Blueprintable)
class GAMEPLAYFRAMEWORK_API UDaInspectableComponent : public USceneComponent, public IDaInspectableInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	UDaInspectableComponent();

	virtual bool AreAssetsLoaded_Implementation() override;
	virtual void LoadAssets_Implementation() override;
	virtual void NativeOnAssetsLoaded() override;
	
protected:

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Components")
	TSoftObjectPtr<USceneComponent> SceneComponent;
	
	UPROPERTY(EditAnywhere, Category = "Inspect")
	TSoftObjectPtr<UStaticMesh> DetailedMeshPtr;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Inspect")
	TObjectPtr<UStaticMesh> LoadedDetailedMesh;
	
	UFUNCTION()
	void OnDetailedMeshLoaded();

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void OnComponentCreated() override;
	
private:
	
	// Asset loading handles
	TSharedPtr<FStreamableHandle> DetailedMeshHandle;
};
