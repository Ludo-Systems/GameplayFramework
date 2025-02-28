// Copyright Dream Awake Solutions LLC

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DaSaveGameSubsystem.generated.h"

class UMVVMViewModelBase;
class UDaSaveGame;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSaveGameSignature, class UDaSaveGame*, SaveObject);

/**
 * 
 */
UCLASS(meta=(DisplayName="SaveGame System"))
class GAMEPLAYFRAMEWORK_API UDaSaveGameSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	
	void SaveSlotData(const FString& LoadSlotName, int32 SlotIndex, bool bClearExisting, TFunction<void(UDaSaveGame*)> SaveDataCallback);
	
	static UDaSaveGame* GetSaveSlotData(const FString& SlotName, int32 SlotIndex);
	static void DeleteSlot(const FString& SlotName, int32 SlotIndex);

	UDaSaveGame* RetrieveInGameSaveData() const;
	void SaveInGameProgressData(TFunction<void(UDaSaveGame*)> SaveDataCallback);
	
	// Restore Serialized data from PlayerState into Player
	void HandleStartingNewPlayer(AController* NewPlayer);

	// Restore Spawn Transform using stored data per PlayerState after being fully initialized
	UFUNCTION(BlueprintCallable)
	bool OverrideSpawnTransform(AController* NewPlayer);
	
	UFUNCTION(BlueprintCallable, Category="SaveGame")
	void WriteSaveGame();

	// Load from disk, optional slot name
	void LoadSaveGame(FString InSlotName="", int32 InSlotIndex=0);

	UPROPERTY(BlueprintAssignable)
	FOnSaveGameSignature OnSaveGameLoaded;

	UPROPERTY(BlueprintAssignable)
	FOnSaveGameSignature OnSaveGameWritten;

	// initialize subsystem, good moment to load in SaveGameSettings variables
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

protected:
	
	UPROPERTY()
	TObjectPtr<UDaSaveGame> CurrentSaveGame;
	
};
