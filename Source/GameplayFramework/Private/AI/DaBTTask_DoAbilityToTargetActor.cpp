// Copyright Dream Awake Solutions LLC


#include "AI/DaBTTask_DoAbilityToTargetActor.h"

#include "AbilitySystemComponent.h"
#include "AIController.h"

#include "AI/DaAICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DaAttributeComponent.h"
#include "CoreGameplayTags.h"

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
		ADaAICharacter* Character = Cast<ADaAICharacter>(MyController->GetPawn());
		if (Character == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetActorKeyName));
		if (TargetActor == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		if (!UDaAttributeComponent::IsActorAlive(TargetActor))
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
