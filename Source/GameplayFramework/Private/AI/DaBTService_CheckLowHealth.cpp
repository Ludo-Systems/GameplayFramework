// Copyright Dream Awake Solutions LLC


#include "AI/DaBTService_CheckLowHealth.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DaAttributeComponent.h"

void UDaBTService_CheckLowHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (ensure(BlackboardComp))
	{
		AAIController* MyController = OwnerComp.GetAIOwner();
		if (ensure(MyController))
		{
			APawn* AIPawn = MyController->GetPawn();
			if (ensure(AIPawn))
			{
				// Check Attribute component to see if Health is below a certain threshold
				UDaAttributeComponent* AttributeComp = UDaAttributeComponent::FindAttributeComponent(AIPawn);
				if (ensure(AttributeComp))
				{
					const bool bHasLowHealth = AttributeComp->GetHealth() <= AttributeComp->GetMaxHealth() * 0.25;
					BlackboardComp->SetValueAsBool(LowHealthKey.SelectedKeyName, bHasLowHealth);
				}
			}
		}
	}
}
