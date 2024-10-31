// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DaPawnData.h"
#include "DaPlayerPawnData.generated.h"

class UDaInputConfig;

/**
 * 
 */
UCLASS(BlueprintType, Const)
class GAMEPLAYFRAMEWORK_API UDaPlayerPawnData : public UDaPawnData
{
	GENERATED_BODY()

public:
	
	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("PlayerPawnData", GetFName());
	}
};
