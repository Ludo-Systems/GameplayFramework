// Copyright Dream Awake Solutions LLC

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GameplayTagContainer.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "DaActorSpawnManager.generated.h"

class ADaPickupItem;
class UEnvQuery;

USTRUCT(BlueprintType)
struct FMonsterInfoRow : public FTableRowBase
{
	GENERATED_BODY()

public:

	FMonsterInfoRow()
	{
		Weight = 1.0f;
		SpawnCost = 5.0f;
		KillReward = 20.0f;
	}
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FPrimaryAssetId MonsterId;

	// Relative chance to pick this monster
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Weight;

	// Points require by gamemode to spawn
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SpawnCost;

	// Credits awarded to killer
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float KillReward;
};


/**
 * 
 */
UCLASS(Blueprintable, Abstract)
class GAMEPLAYFRAMEWORK_API UDaActorSpawnManager : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	virtual void StartSpawning();
	
protected:
	
	UPROPERTY(EditDefaultsOnly, Category="Environment Query")
	TObjectPtr<UEnvQuery> SpawnQuery;
	
	virtual void OnSpawnQueryCompleted(TSharedPtr<FEnvQueryResult> Result);

};


/**
 * 
 */
UCLASS(Blueprintable)
class GAMEPLAYFRAMEWORK_API UDaAISpawnManager : public UDaActorSpawnManager
{
	GENERATED_BODY()

public:
	
	virtual void StartSpawning() override;

	UFUNCTION(BlueprintCallable)
	void KillAllBots();
	
protected:
	
	UPROPERTY(EditDefaultsOnly, Category="AI")
	TObjectPtr<UDataTable> MonsterTable;

	UPROPERTY(EditDefaultsOnly, Category="AI")
	TObjectPtr<UCurveFloat> DifficultyCurve;

	// Values > 0 will override DifficultyCurve. Note: Leave at 0 to use Difficulty Curve
	UPROPERTY(EditDefaultsOnly, Category="AI")
	uint32 MaxBotsOverride;

	UPROPERTY(EditDefaultsOnly, Category="AI")
	float SpawnTimerInterval = 2.0f;

	FTimerHandle TimerHandle_SpawnBots;

	virtual void OnSpawnQueryCompleted(TSharedPtr<FEnvQueryResult> Result) override;
	
	UFUNCTION()
	void SpawnBotTimerElapsed();
	
	void OnMonsterLoaded(FPrimaryAssetId LoadedId, FVector SpawnLocation);
	
};

/**
 * 
 */
UCLASS(Blueprintable)
class GAMEPLAYFRAMEWORK_API UDaPickupItemSpawnManager : public UDaActorSpawnManager
{
	GENERATED_BODY()

public:
	
	virtual void StartSpawning() override;
	
protected:
	
	UPROPERTY(EditDefaultsOnly, Category="Pickups")
	TMap<FGameplayTag, TSubclassOf<ADaPickupItem>> ItemClasses;

	virtual void OnSpawnQueryCompleted(TSharedPtr<FEnvQueryResult> Result) override;
};
