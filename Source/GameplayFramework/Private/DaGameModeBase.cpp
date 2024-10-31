// Copyright Dream Awake Solutions LLC

#include "DaGameModeBase.h"
#include "DaCharacter.h"
#include "DaPlayerState.h"
#include "DaSaveGameSubsystem.h"
#include "Kismet/GameplayStatics.h"

ADaGameModeBase::ADaGameModeBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bAutoRespawnPlayer = true;
	bRespawnAtLastSaveLocation = false;
	RespawnDelay = 3.0f;
}

void ADaGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	UDaSaveGameSubsystem* SG = GetGameInstance()->GetSubsystem<UDaSaveGameSubsystem>();

	FString SelectedSaveSlot = UGameplayStatics::ParseOption(Options, "SaveGame");
	SG->LoadSaveGame(SelectedSaveSlot);
}

void ADaGameModeBase::OnActorKilled(AActor* VictimActor, AActor* KillerActor)
{
	//LogOnScreen(this, FString::Printf(TEXT("ADaGameModeBase::OnActorKilled VictimActor: (%s) KillerActor: (%s)"), *GetNameSafe(VictimActor), *GetNameSafe(KillerActor)));
	//LOG("ADaGameModeBase::OnActorKilled VictimActor: (%s) KillerActor: (%s)", *GetNameSafe(VictimActor), *GetNameSafe(KillerActor));

	// player dies -> timer elapsed, respawn player
	ADaCharacter* Player = Cast<ADaCharacter>(VictimActor);
	if (Player)
	{
		if (bAutoRespawnPlayer)
		{
			FTimerHandle TimerHandle_RespawnDelay;
			FTimerDelegate Delegate;
			Delegate.BindUObject(this, &ThisClass::RespawnPlayerElapsed, Player->GetController());
			GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate, RespawnDelay, false);
		}
		
		// Store time if it was better than previous record
		ADaPlayerState* PS = Player->GetPlayerState<ADaPlayerState>();
		if (PS)
		{
			PS->UpdatePersonalRecord(GetWorld()->TimeSeconds);
		}
		
		// AutoSave on Player Death
		UDaSaveGameSubsystem* SG = GetGameInstance()->GetSubsystem<UDaSaveGameSubsystem>();
		SG->WriteSaveGame();
	}
}

void ADaGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
	if (ensure(Controller))
	{
		//detach
		Controller->UnPossess();

		//respawn
		RestartPlayer(Controller);
	}
}

void ADaGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	UDaSaveGameSubsystem* SG = GetGameInstance()->GetSubsystem<UDaSaveGameSubsystem>();
	SG->HandleStartingNewPlayer(NewPlayer);
	
	// // Will call BeginPlaying State in Player Controller so make sure our data is setup before this calls super
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
	
	// Override spawn location
	if (bRespawnAtLastSaveLocation)
	{
		SG->OverrideSpawnTransform(NewPlayer);
	}
}

