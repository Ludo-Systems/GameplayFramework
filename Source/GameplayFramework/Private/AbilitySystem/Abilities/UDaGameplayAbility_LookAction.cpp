// Copyright Dream Awake Solutions LLC


#include "AbilitySystem/Abilities/UDaGameplayAbility_LookAction.h"

#include "DaCharacter.h"
#include "CoreGameplayTags.h"

void UUDaGameplayAbility_LookAction::OnTriggeredInputAction(const FInputActionValue& Value)
{
	if (ADaCharacter* PlayerCharacter = Cast<ADaCharacter>(GetAvatarActorFromActorInfo()))
	{
		const FVector2d MouseVec = Value.Get<FVector2d>();

		PlayerCharacter->AddControllerYawInput(MouseVec.X);
		PlayerCharacter->AddControllerPitchInput(MouseVec.Y);
	}
}

UUDaGameplayAbility_LookAction::UUDaGameplayAbility_LookAction(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	StartupActivationTag = CoreGameplayTags::TAG_Input_LookMouse;
}
