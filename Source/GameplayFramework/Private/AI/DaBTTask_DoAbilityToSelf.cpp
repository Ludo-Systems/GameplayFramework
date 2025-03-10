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
		ACharacter* Character = Cast<ACharacter>(MyController->GetPawn());
		if (Character == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		if (IAbilitySystemInterface* ASCCharacter = Cast<IAbilitySystemInterface>(Character))
		{
			UAbilitySystemComponent* ASC = ASCCharacter->GetAbilitySystemComponent();
			if(ASC->TryActivateAbilitiesByTag(FGameplayTagContainer(AbilityToExecute)))
			{
				return EBTNodeResult::Succeeded;
			}
		}
	}

	return EBTNodeResult::Failed;
}
