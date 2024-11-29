// Copyright Dream Awake Solutions LLC


#include "AI/DaBTTask_DoAbilityToSelf.h"

#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "AI/DaAICharacter.h"
#include "CoreGameplayTags.h"

UDaBTTask_DoAbilityToSelf::UDaBTTask_DoAbilityToSelf()
{
	AbilityToExecute = CoreGameplayTags::TAG_Action_Heal;
}

EBTNodeResult::Type UDaBTTask_DoAbilityToSelf::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* MyController = OwnerComp.GetAIOwner();
	if (ensure(MyController))
	{
		ADaAICharacter* Character = Cast<ADaAICharacter>(MyController->GetPawn());
		if (Character == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		UAbilitySystemComponent* ASC = Character->GetAbilitySystemComponent();
		if(ASC->TryActivateAbilitiesByTag(FGameplayTagContainer(AbilityToExecute)))
		{
			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}
