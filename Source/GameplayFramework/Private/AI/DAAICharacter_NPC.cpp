// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/DAAICharacter_NPC.h"

#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "CoreGameplayTags.h"
#include "AbilitySystem/DaAbilitySystemComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ADAAICharacter_NPC::ADAAICharacter_NPC()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CharacterIDGameplayTag = CoreGameplayTags::TAG_Character_ID;
	CharacterTypeGameplayTag = CoreGameplayTags::TAG_Character_Type_AI_NPC;

	AbilitySystemComponent = CreateDefaultSubobject<UDaAbilitySystemComponent>(TEXT("AbilityComp"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	TargetActorKey = "TargetActor";
}

void ADAAICharacter_NPC::BeginPlay()
{
	Super::BeginPlay();
	
	InitAbilitySystem();
}

void ADAAICharacter_NPC::InitAbilitySystem()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UDaAbilitySystemComponent>(GetAbilitySystemComponent())->AbilityActorInfoSet();

	// Pretty much always targets the player if alive
	SetTargetActor(UGameplayStatics::GetPlayerPawn(this, 0));

	AbilitySystemComponent->InitAbilitiesWithPawnData(PawnData);
	
	InitDefaultAttributes();
}

UAbilitySystemComponent* ADAAICharacter_NPC::GetAbilitySystemComponent() const
{
	return Cast<UAbilitySystemComponent>(AbilitySystemComponent);
}

void ADAAICharacter_NPC::InitDefaultAttributes() const
{
	if (DefaultAttributes)
		ApplyEffectToSelf(DefaultAttributes, 1.f);
}

void ADAAICharacter_NPC::ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& GameplayEffectClass,
                                             const float Level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	if (GameplayEffectClass)
	{
		FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
		ContextHandle.AddSourceObject(this);
		const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
		GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
	}
}

AActor* ADAAICharacter_NPC::GetTargetActor()
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		return Cast<AActor>(AIController->GetBlackboardComponent()->GetValueAsObject(TargetActorKey));
	}
	return nullptr;
}

void ADAAICharacter_NPC::SetTargetActor(AActor* NewTarget)
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		AIController->GetBlackboardComponent()->SetValueAsObject(TargetActorKey, NewTarget);
	}
}


