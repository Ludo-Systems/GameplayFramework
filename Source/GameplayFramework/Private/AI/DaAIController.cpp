// Copyright Dream Awake Solutions LLC


#include "AI/DaAIController.h"

#include "AbilitySystemInterface.h"
#include "AbilitySystem/DaAbilitySystemComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"


void ADaAIController::BeginPlay()
{
	Super::BeginPlay();

	// Will only work from BeginPlay if AI actor is spawned in the world, not placed in map
	// ASC Key will be NULL in that case.
	AddPlayerPawnAbilitySystemComponentToBlackboard();
	
	if (bAutoRunBehaviorTree && ensureMsgf(BehaviorTree, TEXT("Behavior Tree is nullptr! Please assign BehaviorTree in your AI Controller")))
	{
		RunBehaviorTree(BehaviorTree);
	}
}

ADaAIController::ADaAIController()
{
	TargetActorKeyName = "TargetActor";
	AbilitySystemComponentKeyName = "ASC";
	bAutoRunBehaviorTree = true;
	bHasAddedAbilitySystemComponentToBlackboard = false;
}

void ADaAIController::AddPlayerPawnAbilitySystemComponentToBlackboard()
{
	UBlackboardComponent* BB = GetBlackboardComponent();
	if (BB && !bHasAddedAbilitySystemComponentToBlackboard)
	{
		APawn* ControlledActor = UGameplayStatics::GetPlayerPawn(this, 0);
		if (ControlledActor)
		{
			if (IAbilitySystemInterface* ASCCharacter = Cast<IAbilitySystemInterface>(ControlledActor))
			{
				UAbilitySystemComponent* ASC =  ASCCharacter->GetAbilitySystemComponent();
				BB->SetValueAsObject(AbilitySystemComponentKeyName, ASC);
				bHasAddedAbilitySystemComponentToBlackboard = true;
			}
		}
	}
}

void ADaAIController::AddTargetActorToBlackBoard(AActor* TargetActor)
{
	UBlackboardComponent* BB = GetBlackboardComponent();
	if (BB && !bHasAddedAbilitySystemComponentToBlackboard)
	{
		if (TargetActor)
		{
			BB->SetValueAsObject(TargetActorKeyName, TargetActor);
		}
	}
}
