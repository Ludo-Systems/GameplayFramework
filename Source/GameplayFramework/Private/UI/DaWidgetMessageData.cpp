// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DaWidgetMessageData.h"

#include "GameplayFramework.h"

FDaUIWidgetMessageData UDaWidgetMessageData::FindMessageDataForTag(const FGameplayTag& MessageTag, bool bLogNotFound) const
{
	for (const FDaUIWidgetMessageData& Info : MessageData)
	{
		if (Info.MessageTag.MatchesTagExact(MessageTag))
		{
			return Info;
		}
	}

	if (bLogNotFound)
	{
		LOG_ERROR("Can't find Data for MessageTag [%s] on Message Data Asset [%s].", *MessageTag.ToString(), *GetNameSafe(this));
	}

	return FDaUIWidgetMessageData();
}
