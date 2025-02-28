// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "DaGameInstanceBase.generated.h"

class UDaSaveGame;
/**
 * 
 */
UCLASS()
class GAMEPLAYFRAMEWORK_API UDaGameInstanceBase : public UGameInstance
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SaveGame")
	TSubclassOf<UDaSaveGame> SaveGameClass;
	
	UPROPERTY()
	FName PlayerStartTag = FName();

	UPROPERTY()
	FString LoadSlotName = FString();

	UPROPERTY()
	int32 LoadSlotIndex = 0;
	
};
