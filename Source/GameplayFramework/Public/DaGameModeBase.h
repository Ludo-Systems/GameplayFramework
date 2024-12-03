// Copyright Dream Awake Solutions LLC

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DaGameModeBase.generated.h"

class AController;

/**
 * 
 */
UCLASS(Config = Game)
class GAMEPLAYFRAMEWORK_API ADaGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:

	// if true will auto respawn if false, let subclasses handle how we respawn or end, example might be go to spectator mode or start menu
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Respawn Ruleset")
	bool bAutoRespawnPlayer;

	// if auto respawn is true, how long to wait between respawns
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Respawn Ruleset")
	float RespawnDelay;

	// respawn at last save if true or a start point if false
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Respawn Ruleset")
	bool bRespawnAtLastSaveLocation;
	
	UFUNCTION()
	void RespawnPlayerElapsed(AController* Controller);

public:

	ADaGameModeBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	// subclass should call super if override to handle respawn behavior but can check if a player or AI was killed and act accordingly
	virtual void OnActorKilled(AActor* VictimActor, AActor* KillerActor);
	
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
};
