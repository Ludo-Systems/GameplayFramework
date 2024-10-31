// Copyright Dream Awake Solutions LLC

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DaQuest.generated.h"

class UDaQuest;
class UDaInputAtom;
class UDaState;

UENUM(BlueprintType)
enum class EQuestCompletion : uint8
{
	EQC_NotStarted,
	EQC_Started,
	EQC_Succeeded,
	EQC_Failed
};

USTRUCT()
struct GAMEPLAYFRAMEWORK_API FQuestInProgress
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	const UDaQuest* Quest;

	UPROPERTY(EditAnywhere)
	EQuestCompletion QuestProgress;

protected:
	// All input for this quest filtered by white/black list
	UPROPERTY(EditAnywhere)
	TArray<UDaInputAtom*> QuestActivities;

public:

	// Returns true if quest was completed by new activity
	bool UpdateQuest(const UObject* ObjectRef, UDaInputAtom* QuestActivity);

	static FQuestInProgress NewQuestInProgress(const UDaQuest* Quest)
	{
		FQuestInProgress NewQIP;
		NewQIP.Quest = Quest;
		NewQIP.QuestProgress = EQuestCompletion::EQC_Started;
		return NewQIP;
	}

	FQuestInProgress() : Quest(nullptr), QuestProgress(EQuestCompletion::EQC_NotStarted) {}
	FQuestInProgress(const UDaQuest* NewQuest, const EQuestCompletion NewQuestProgress = EQuestCompletion::EQC_Started) : Quest(NewQuest), QuestProgress(NewQuestProgress) {}
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMEPLAYFRAMEWORK_API UDaQuestComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Add to our activity log. Auto checks to see unfinished quests are now complete.
	UFUNCTION(BlueprintCallable, Category="Quests")
	void UpdateQuests(UDaInputAtom* QuestActivity);

	// Add a new quest in progress otr begin if its already on the list and not yet started
	UFUNCTION(BlueprintCallable, Category="Quests")
	bool BeginQuest(const UDaQuest* Quest);
	
protected:

	// master list of all quest related things we've done.
	UPROPERTY(EditAnywhere)
	TArray<UDaInputAtom*> QuestActivities;
	
	// List of quests in our current game or area
	UPROPERTY(EditAnywhere)
	TArray<FQuestInProgress> QuestList;
};

/**
 * 
 */
UCLASS()
class GAMEPLAYFRAMEWORK_API UDaQuest : public UDataAsset
{
	GENERATED_BODY()

public:

	// localizable quest name
	UPROPERTY(EditAnywhere)
	FText QuestName;

	// If this machine accepts our quest activity log, the quest is succesful
	UPROPERTY(EditAnywhere)
	UDaState* QuestStateMachine;

	// If true, the InputList is a blacklist, otherwise its a whitelist
	UPROPERTY(EditAnywhere)
	uint32 bInputBlackList : 1;

	// Filter InputAtoms this quest recognizes (blacklist or whitelist depending on bInputBlackList)
	UPROPERTY(EditAnywhere)
	TArray<UDaInputAtom*> InputList;

	virtual void OnSucceed(UDaQuestComponent* QuestStatus) const;
	virtual void OnFailed(UDaQuestComponent* QuestStatus) const;
};

UCLASS()
class GAMEPLAYFRAMEWORK_API UDaQuestWithResult : public UDaQuest
{
	GENERATED_BODY()

public:

	virtual void OnSucceed(UDaQuestComponent* QuestStatus) const override;
	virtual void OnFailed(UDaQuestComponent* QuestStatus) const override;

protected:
	// the quests in this list will go from NotStarted to Started if current quest succeeds
	UPROPERTY(EditAnywhere)
	TArray<UDaQuest*> SuccessQuests;

	// added if quests succeeds
	UPROPERTY(EditAnywhere)
	TArray<UDaInputAtom*> SuccessInputs;

	// the quests in this list will go from NotStarted to Started if current quest succeeds
	UPROPERTY(EditAnywhere)
	TArray<UDaQuest*> FailureQuests;

	// added if quests fails
	UPROPERTY(EditAnywhere)
	TArray<UDaInputAtom*> FailureInputs;
};
