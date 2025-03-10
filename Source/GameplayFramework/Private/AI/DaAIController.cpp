// Copyright Dream Awake Solutions LLC


#include "AI/DaAIController.h"

#include "AbilitySystemInterface.h"
#include "AbilitySystem/DaAbilitySystemComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

void ADaAIController::BeginPlay()
{
	Super::BeginPlay();

	UBlackboardComponent* BB = GetBlackboardComponent();

	// Set the ability system component of this pawn so its available in Blackboard
	APawn* ControlledActor = UGameplayStatics::GetPlayerPawn(this, 0);
	if (ControlledActor)
	{
		if (IAbilitySystemInterface* ASCCharacter = Cast<IAbilitySystemInterface>(ControlledActor))
		{
			UAbilitySystemComponent* ASC =  ASCCharacter->GetAbilitySystemComponent();
			if (BB)
			{
				BB->SetValueAsObject("ASC", ASC);
			}
		}
	}
	
	if (ensureMsgf(BehaviorTree, TEXT("Behavior Tree is nullptr! Please assign BehaviorTree in your AI Controller")))
	{
		RunBehaviorTree(BehaviorTree);
	}
}

