// Copyright Dream Awake Solutions LLC

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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Pawn")
	TSubclassOf<AActor> PawnClass;
	
	// Ability sets to grant to this pawn's ability system.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Abilities")
	TArray<TObjectPtr<UDaAbilitySet>> AbilitySets;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|UI")
	TObjectPtr<UTexture2D> Icon;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("PawnData", GetFName());
	}
};
