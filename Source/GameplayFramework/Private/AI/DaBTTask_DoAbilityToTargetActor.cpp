// Copyright Dream Awake Solutions LLC


#include "AI/DaBTTask_DoAbilityToTargetActor.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "DaAttributeComponent.h"
#include "CoreGameplayTags.h"
#include "GameFramework/Character.h"

UDaBTTask_DoAbilityToTargetActor::UDaBTTask_DoAbilityToTargetActor()
{
	// set a reasonable default
	AbilityToExecute = CoreGameplayTags::TAG_Action_PrimaryAttack;
	TargetActorKeyName = "TargetActor";
}

EBTNodeResult::Type UDaBTTask_DoAbilityToTargetActor::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* MyController = OwnerComp.GetAIOwner();
	if (ensure(MyController))
	{
		AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetActorKeyName));
		if (TargetActor == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		if (!UDaAttributeComponent::IsActorAlive(TargetActor))
		{
			return EBTNodeResult::Failed;
		}

		if (IAbilitySystemInterface* TargetActorWithASC = Cast<IAbilitySystemInterface>(TargetActor))
		{
			UAbilitySystemComponent* ASC = TargetActorWithASC->GetAbilitySystemComponent();
			if(ASC->TryActivateAbilitiesByTag(FGameplayTagContainer(AbilityToExecute)))
			{
				return EBTNodeResult::Succeeded;
			}
		}
	}

	return EBTNodeResult::Failed;
}
