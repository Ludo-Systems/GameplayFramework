// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CEEditionTemplateData.generated.h"

/**
 * 
 */
UCLASS()
class COLLECTIBLES_API UCEEditionTemplateData : public UPrimaryDataAsset
{
	GENERATED_BODY()

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("EditionTemplateData", GetFName());
	}
};
