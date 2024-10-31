// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/DaAIController.h"

#include "AbilitySystem/DaAbilitySystemComponent.h"
#include "AI/DaAICharacter.h"
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
		if (ADaAICharacter* AICharacter = Cast<ADaAICharacter>(ControlledActor))
		{
			UAbilitySystemComponent* ASC =  AICharacter->GetAbilitySystemComponent();
			BB->SetValueAsObject("ASC", ASC);
		}
	}
	
	if (ensureMsgf(BehaviorTree, TEXT("Behavior Tree is nullptr! Please assign BehaviorTree in your AI Controller")))
	{
		RunBehaviorTree(BehaviorTree);
	}
}

