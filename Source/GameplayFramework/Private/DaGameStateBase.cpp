// Fill out your copyright notice in the Description page of Project Settings.


#include "DaGameStateBase.h"

#include "AbilitySystem/DaAbilitySystemComponent.h"

ADaGameStateBase::ADaGameStateBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<UDaAbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
}

UAbilitySystemComponent* ADaGameStateBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ADaGameStateBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	check(AbilitySystemComponent);
	AbilitySystemComponent->InitAbilityActorInfo(/*Owner=*/ this, /*Avatar=*/ this);
}
