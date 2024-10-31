// Copyright Dream Awake Solutions LLC


#include "DaQuest.h"
#include "DaState.h"
#include "GameplayFramework.h"

bool FQuestInProgress::UpdateQuest(const UObject* ObjectRef, UDaInputAtom* QuestActivity)
{
	// Only log activity to valid, in-progress quests. Check the blacklist/whitelist before logging
	if (Quest && (QuestProgress == EQuestCompletion::EQC_Started) && (Quest->bInputBlackList != Quest->InputList.Contains(QuestActivity)))
	{
		QuestActivities.Add(QuestActivity);
		FStateMachineResult QuestResult = Quest->QuestStateMachine->RunState(ObjectRef, QuestActivities);
		switch (QuestResult.CompletionType)
		{
		case EStateMachineCompletionType::Accepted:
			QuestProgress = EQuestCompletion::EQC_Succeeded;
			return true;
		case EStateMachineCompletionType::Rejected:
			QuestProgress = EQuestCompletion::EQC_Failed;
			return true;
			// Quest NotAccepted: Still in progress, no update needed
		default:
			return false;
		}
	}
	return false;
}

void UDaQuestComponent::UpdateQuests(UDaInputAtom* QuestActivity)
{
	TArray<int32> RecentlyCompletedQuests;

	// update master list of everything we've ever done
	QuestActivities.Add(QuestActivity);

	// Update individual quests (if they care about this activity) and see if they are complete.
	for (int32 i = QuestList.Num() - 1; i >= 0; --i)
	{
		if (QuestList[i].UpdateQuest(this, QuestActivity))
		{
			RecentlyCompletedQuests.Add(i);
		}
	}

	// process completed quests after completing all quests
	// this way quests cant inject out of order
	for (int32 i = RecentlyCompletedQuests.Num() - 1; i >= 0; --i)
	{
		FQuestInProgress& QIP = QuestList[RecentlyCompletedQuests[i]];
		if (QIP.QuestProgress == EQuestCompletion::EQC_Succeeded)
		{
			QIP.Quest->OnSucceed(this);
		} else
		{
			QIP.Quest->OnFailed(this);
		}
		// dont think we need to remove anything as this will go away with function scope, but some other code had this so keeping commented out in case its needed
		//RecentlyCompletedQuests.RemoveAtSwap(i);
	}
}

bool UDaQuestComponent::BeginQuest(const UDaQuest* Quest)
{
	for(FQuestInProgress& QIP: QuestList)
	{
		if (QIP.Quest == Quest)
		{
			if (QIP.QuestProgress == EQuestCompletion::EQC_NotStarted)
			{
				LOG_WARNING("Changing Quest \"%s\" to started status", *QIP.Quest->QuestName.ToString());
				QIP.QuestProgress = EQuestCompletion::EQC_Started;
				return true;
			}
			LOG_WARNING("Quest \"%s\" already in list", *QIP.Quest->QuestName.ToString());
			return false;
		}
	}

	LOG_WARNING("Adding Quest \"%s\" to list and starting", *Quest->QuestName.ToString());
	QuestList.Add(FQuestInProgress::NewQuestInProgress(Quest));
	return true;
}

void UDaQuest::OnSucceed(UDaQuestComponent* QuestStatus) const
{
	LOG_WARNING("Quest \"%s\" Succeeded!", *QuestName.ToString());
}

void UDaQuest::OnFailed(UDaQuestComponent* QuestStatus) const
{
	LOG_WARNING("Quest \"%s\" Failed!", *QuestName.ToString());
}

void UDaQuestWithResult::OnSucceed(UDaQuestComponent* QuestStatus) const
{
	Super::OnSucceed(QuestStatus);

	for (const UDaQuest* SuccessQuest : SuccessQuests)
	{
		QuestStatus->BeginQuest(SuccessQuest);
	}

	for (int32 i = 0; i < SuccessInputs.Num(); ++i)
	{
		QuestStatus->UpdateQuests(SuccessInputs[i]);
	}
	
}

void UDaQuestWithResult::OnFailed(UDaQuestComponent* QuestStatus) const
{
	Super::OnFailed(QuestStatus);

	for (const UDaQuest* FailureQuest : FailureQuests)
	{
		QuestStatus->BeginQuest(FailureQuest);
	}

	for (int32 i = 0; i < FailureInputs.Num(); ++i)
	{
		QuestStatus->UpdateQuests(FailureInputs[i]);
	}
}
