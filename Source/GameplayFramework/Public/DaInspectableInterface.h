// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DaInspectableInterface.generated.h"

DECLARE_MULTICAST_DELEGATE(FInspectableDetailLoaded);

UINTERFACE(BlueprintType)
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
	virtual void NativeOnAssetsLoaded() = 0;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool AreAssetsLoaded();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void LoadAssets();
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnAssetsLoaded();
	
	FInspectableDetailLoaded OnAssetsLoadedDelegate;
};
