// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CEWearTemplateData.generated.h"

/**
 * 
 */
UCLASS()
class COLLECTIBLES_API UCEWearTemplateData : public UPrimaryDataAsset
{
	GENERATED_BODY()

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("WearTemplateData", GetFName());
	}
};
