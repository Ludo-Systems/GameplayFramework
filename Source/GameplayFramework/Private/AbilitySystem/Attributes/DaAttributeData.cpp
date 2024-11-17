// Copyright Dream Awake Solutions LLC


#include "AbilitySystem/Attributes/DaAttributeData.h"

#include "GameplayFramework.h"

FDaAttributeData UDaAttributeInfo::FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound) const
{
	for (const FDaAttributeData& Info : AttributeInfo)
	{
		if (Info.AttributeTag.MatchesTagExact(AttributeTag))
		{
			return Info;
		}
	}

	if (bLogNotFound)
	{
		LOG_ERROR("Can't find Info struct for Attribute Tag [%s] on AttributeInfo [%s].", *AttributeTag.ToString(), *GetNameSafe(this));
	}

	return FDaAttributeData();
}
