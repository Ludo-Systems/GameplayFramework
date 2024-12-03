// Copyright Dream Awake Solutions LLC

#pragma once

#include "CoreMinimal.h"
#include "DaInputConfig.h"
#include "EnhancedInputComponent.h"
#include "DaInputComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMEPLAYFRAMEWORK_API UDaInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:

	template<class UserClass, typename PressFuncType, typename ReleasedFuncType, typename HeldFuncType>
	void BindAbilityActions(const UDaInputConfig* InputConfig, UserClass* Object, PressFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc);
	
	UDaInputComponent();
	
};

template <class UserClass, typename PressFuncType, typename ReleasedFuncType, typename HeldFuncType>
void UDaInputComponent::BindAbilityActions(const UDaInputConfig* InputConfig, UserClass* Object,
	PressFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc)
{
	check(InputConfig);

	for (const FDaInputAction& Action: InputConfig->AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag.IsValid())
		{
			if (PressedFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Started, Object, PressedFunc, Action.InputTag);
			}

			if (ReleasedFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag);
			}
			
			if (HeldFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, Action.InputTag);
			}
		}
	}
}
