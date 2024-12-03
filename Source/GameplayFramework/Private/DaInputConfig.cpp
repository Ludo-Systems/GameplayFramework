// Copyright Dream Awake Solutions LLC


#include "DaInputConfig.h"

#include "GameplayFramework.h"


const UInputAction* UDaInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag, const bool bLogNotFound) const
{
	for (const FDaInputAction& Action: AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag == InputTag)
		{
			return Action.InputAction;
		}
	}

	if (bLogNotFound)
	{
		LOG("UDaInputConfig: Could not find AbilityInputAction for InputTag [%s] on InputConfig [%s]", *InputTag.ToString(), *GetNameSafe(this));
	}

	return nullptr;
}
