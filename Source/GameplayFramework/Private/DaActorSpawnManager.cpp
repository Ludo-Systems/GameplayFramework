// Copyright Dream Awake Solutions LLC


#include "DaActorSpawnManager.h"

#include "CoreGameplayTags.h"
#include "GameplayFramework.h"
#include "AI/DaAICharacter.h"
#include "DaAttributeComponent.h"
#include "DaPickupItem.h"

#include "EngineUtils.h"
#include "AbilitySystem/DaAbilitySystemComponent.h"
#include "DaPawnData.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "EnvironmentQuery/EnvQueryManager.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("da.SpawnBots"), true, TEXT("Enable Spawning of Bots with Timer"), ECVF_Cheat);
static TAutoConsoleVariable<bool> CVarDebugSpawnBots(TEXT("da.DrawDebugSpawnBots"), false, TEXT("Draw Debug Spheres showing location where Bots spawned"), ECVF_Cheat);
static TAutoConsoleVariable<bool> CVarDebugSpawnItems(TEXT("da.DrawDebugSpawnItems"), false, TEXT("Draw Debug Spheres showing location where Items spawned"), ECVF_Cheat);


void UDaActorSpawnManager::StartSpawning(FString LevelName)
{
	// Store for later use.
	CurrentLevelName = LevelName;
}

void UDaActorSpawnManager::OnSpawnQueryCompleted(TSharedPtr<FEnvQueryResult> Result)
{
	// Virtual only
}


/// ----------------- AI Spawn Manager ---------------- ///

void UDaAISpawnManager::StartSpawning(FString LevelName)
{
	Super::StartSpawning(LevelName);
	
	if (TimerHandle_SpawnBots.IsValid())
	{
		// Already spawning bots.
		return;
	}

	// Continuous timer to spawn in more bots.
	// Actual amount of bots and whether its allowed to spawn determined by spawn logic later in the chain...
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_SpawnBots, this, &UDaAISpawnManager::SpawnBotTimerElapsed, SpawnTimerInterval, true);
}

void UDaAISpawnManager::SpawnBotTimerElapsed()
{
	if (!CVarSpawnBots.GetValueOnGameThread())
	{
		LOG_WARNING("Bot Spawning Disabled via CVar 'CVarSpawnBots'.");
		return;
	}
	
	int32 NumberOfAliveBots = 0;
	for(ADaAICharacter* Bot : TActorRange<ADaAICharacter>(GetWorld()))
	{
		UDaAttributeComponent* AttribComp = UDaAttributeComponent::FindAttributeComponent(Bot);
		if (ensure(AttribComp) && !AttribComp->IsDeadOrDying())
		{
			NumberOfAliveBots++;
		}
	}

	LOG("Found %i alive bots", NumberOfAliveBots);
	
	float MaxBotCount = MaxBotsOverride;
	if (MaxBotsOverride == 0 && DifficultyCurve)
	{
		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}

	if (NumberOfAliveBots >= MaxBotCount)
	{
		LOG("At Max Capacity, %i alive bots. Skipping spawn", NumberOfAliveBots);
		return;
	}

	FEnvQueryRequest Request(SpawnQuery, this);
	Request.Execute(EEnvQueryRunMode::RandomBest5Pct, this, &UDaAISpawnManager::OnSpawnQueryCompleted);
}

void UDaAISpawnManager::OnSpawnQueryCompleted(TSharedPtr<FEnvQueryResult> Result)
{
	FEnvQueryResult* QueryResult = Result.Get();
	if (!QueryResult->IsSuccessful())
	{
		//UE_LOG(DragonRogue, Warning, TEXT("Spawn bot EQS Query Failed"));
		LOG_WARNING("Spawn bot EQS Query Failed");
		return;
	}

	// Retrieve all possible locations that passed the query
	TArray<FVector> Locations;
	QueryResult->GetAllAsLocations(Locations);
	if (Locations.IsValidIndex(0) && MonsterTable)
	{
		//LogOnScreen(this, "Begin Loading", FColor::Yellow);

		TArray<FMonsterInfoRow*> Rows;
		MonsterTable->GetAllRows("", Rows);

		//Get Random Enemy
		int32 RandomIndex = FMath::RandRange(0, Rows.Num() - 1);
		FMonsterInfoRow* SelectedRow = Rows[RandomIndex];

		UAssetManager& AssMan = UAssetManager::Get();

		TArray<FName> Bundles; 
		FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(this, &UDaAISpawnManager::OnMonsterLoaded, SelectedRow->MonsterId, Locations[0]);
		AssMan.LoadPrimaryAsset(SelectedRow->MonsterId, Bundles, Delegate);
	}
}

void UDaAISpawnManager::OnMonsterLoaded(FPrimaryAssetId LoadedId, FVector SpawnLocation)
{
	UAssetManager& AssMan = UAssetManager::Get();

	if (const UDaPawnData* PawnData = Cast<UDaPawnData>(AssMan.GetPrimaryAssetObject(LoadedId)))
	{
		if (const AActor* Monster = GetWorld()->SpawnActor<AActor>(PawnData->PawnClass, SpawnLocation, FRotator::ZeroRotator))
		{
			//LogOnScreen(this, FString::Printf(TEXT("Spawned Enemy: %s (%s)"), *GetNameSafe(Monster), *GetNameSafe(PawnData)));

			UDaAbilitySystemComponent* ASC = Monster->FindComponentByClass<UDaAbilitySystemComponent>();
			ASC->InitAbilitiesWithPawnData(PawnData);
		}
	}
	
	if (CVarDebugSpawnBots.GetValueOnGameThread())
	{
		DrawDebugSphere(GetWorld(), SpawnLocation, 50.0f, 20, FColor::Blue, false, 60.f);
	}
}

void UDaAISpawnManager::KillAllBots()
{
	for(const ADaAICharacter* Bot : TActorRange<ADaAICharacter>(GetWorld()))
	{
		UDaAttributeComponent* AttribComp = UDaAttributeComponent::FindAttributeComponent(Bot);
		if (ensure(AttribComp) && !AttribComp->IsDeadOrDying())
		{
			AttribComp->DamageSelfDestruct(); 
		}
	}
}


/// ----------------- Pickup Item Spawn Manager ---------------- ///

void UDaPickupItemSpawnManager::StartSpawning(FString LevelName)
{
	Super::StartSpawning(LevelName);

	// Skip the Blueprint wrapper and use the direct C++ option which the Wrapper uses as well
	FEnvQueryRequest Request(SpawnQuery, this);
	Request.Execute(EEnvQueryRunMode::AllMatching, this, &UDaPickupItemSpawnManager::OnSpawnQueryCompleted);
}

void UDaPickupItemSpawnManager::OnSpawnQueryCompleted(TSharedPtr<FEnvQueryResult> Result)
{
	FEnvQueryResult* QueryResult = Result.Get();
	if (!QueryResult->IsSuccessful())
	{
		LOG_WARNING("Spawn ITEM EQS Query Failed");
		return;
	}

	TArray<FGameplayTag> ItemTags;
	int32 TagCount = ItemClasses.GetKeys(ItemTags);
	if (TagCount==0){
		LOG_WARNING("Spawn ITEM Failed, Tags and Item classes not Defined in Gamemode");
		return;
	}
	
	int32 PickupTypes = ItemTags.Num();
	int32 CurrentType = 0;
	if (PickupTypes==0)
	{
		LOG_WARNING("Spawn ITEM Failed, Item classes not Defined in Gamemode");
		return;
	}

	//@TODO: Currently relying on EQS to filter out items, but we could just a get a grid and use something like "Algo/RandomShuffle.h" and pick the first X items from the array
	// Also could introduce a spawn actor class that can be dropped in the map, instead of relying on GameMode's location (the QueryContext)
	
	// Retrieve all possible locations that passed the query
	TArray<FVector> Locations;
	QueryResult->GetAllAsLocations(Locations);
	for (FVector Loc : Locations)
	{
		FGameplayTag CurrentTag = ItemTags[CurrentType++];
		if (CurrentType == PickupTypes)
		{
			CurrentType=0;
		}
		
		if (!CurrentTag.MatchesTag(CoreGameplayTags::TAG_Pickup))
		{
			LOG_WARNING("Non Matching Pickup GameplayTag: %s", *CurrentTag.ToString());
			continue;
		}
		
		// Raise above ground level
		Loc.Z += 100.0f;
		GetWorld()->SpawnActor<ADaPickupItem>(ItemClasses[CurrentTag], Loc, FRotator::ZeroRotator);
		
		// Debug
		if (CVarDebugSpawnItems.GetValueOnGameThread())
		{
			DrawDebugSphere(GetWorld(), Loc, 50.0f, 20, FColor::Yellow, false, 60.f);
			FString DebugMsg = FString::Printf(TEXT("Spawned %s at Location: %s"), *CurrentTag.ToString(), *Loc.ToString());
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, DebugMsg);
		}
	}
}




