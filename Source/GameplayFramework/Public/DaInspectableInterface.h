// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DaInspectableInterface.generated.h"

UINTERFACE(Blueprintable)
class GAMEPLAYFRAMEWORK_API UDaInspectableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GAMEPLAYFRAMEWORK_API IDaInspectableInterface
{
	GENERATED_BODY()

public:
	// Core inspection state changes that actors might want to handle
	UFUNCTION(BlueprintNativeEvent, Category = "Inspectable")
	void OnInspectionStarted(APawn* InstigatorPawn);

	UFUNCTION(BlueprintNativeEvent, Category = "Inspectable")
	void OnInspectionEnded(APawn* InstigatorPawn);

	// Allow actors to control what mesh is inspected
	UFUNCTION(BlueprintNativeEvent, Category = "Inspectable")
	UStaticMeshComponent* GetPreviewMeshComponent() const;
    
	UFUNCTION(BlueprintNativeEvent, Category = "Inspectable")
	UStaticMeshComponent* GetDetailedMeshComponent() const;

};
