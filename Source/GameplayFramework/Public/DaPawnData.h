// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DaPawnData.generated.h"

class UDaAbilitySet;

/**
 * 
 */
UCLASS(BlueprintType, Const)
class GAMEPLAYFRAMEWORK_API UDaPawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	// Ability sets to grant to this pawn's ability system.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DA|Abilities")
	TArray<TObjectPtr<UDaAbilitySet>> AbilitySets;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DA|UI")
	TObjectPtr<UTexture2D> Icon;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("PawnData", GetFName());
	}
};
