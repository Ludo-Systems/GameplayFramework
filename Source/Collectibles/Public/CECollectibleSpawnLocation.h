// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CECollectibleData.h"
#include "GameFramework/Actor.h"
#include "CECollectibleSpawnLocation.generated.h"

UCLASS(Blueprintable)
class COLLECTIBLES_API ACECollectibleSpawnLocation : public AActor
{
	GENERATED_BODY()

public:
	ACECollectibleSpawnLocation();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ECollectibleSpawnLocations> SpawnLocationType = ECollectibleSpawnLocations::UnsortedInventoryLocation;

};
