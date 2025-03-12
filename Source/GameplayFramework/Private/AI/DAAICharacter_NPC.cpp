// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/DAAICharacter_NPC.h"

#include "AbilitySystemComponent.h"
#include "CoreGameplayTags.h"
#include "AbilitySystem/DaAbilitySystemComponent.h"
#include "AbilitySystem/Attributes/DaAttributeData.h"
#include "AbilitySystem/Attributes/DaBaseAttributeSet.h"
#include "AI/DaNPCDialogData.h"

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

	NPCDialogDataClass = UDaNPCDialogData::StaticClass();
}

void ADAAICharacter_NPC::BeginPlay()
{
	// Set this up before calling super so we can cast to subclass in blueprints begin play
	NPCDialogData = NewObject<UDaNPCDialogData>(this, NPCDialogDataClass);
	
	Super::BeginPlay();

	InitAbilitySystem();
}

void ADAAICharacter_NPC::UpgradeAttribute(const FGameplayTag& AttributeTag, int32 Amount)
{
	AbilitySystemComponent->UpgradeAttribute(AttributeTag, Amount);
}

void ADAAICharacter_NPC::InitAbilitySystem()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UDaAbilitySystemComponent>(GetAbilitySystemComponent())->AbilityActorInfoSet();

	AbilitySystemComponent->InitAbilitiesWithPawnData(PawnData);
	
	InitDefaultAttributes();
}

UAbilitySystemComponent* ADAAICharacter_NPC::GetAbilitySystemComponent() const
{
	return Cast<UAbilitySystemComponent>(AbilitySystemComponent);
}

void ADAAICharacter_NPC::InitDefaultAttributes() const
{
	for (auto It = AttributeSetTags.CreateConstIterator(); It; ++It)
	{
		if (!It->IsValid()) continue;
		UDaBaseAttributeSet* AttributeSet = AbilitySystemComponent->GetAttributeSetForTag(*It);
		if (AttributeSet)
		{
			for (auto& Pair: AttributeSet->TagsToAttributes)
			{
				AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
					[this, Pair, AttributeSet](const FOnAttributeChangeData& Data)
				{
					BroadcastAttributeInfo(Pair.Key, Pair.Value(), AttributeSet);
				});
			}
		}
	}
	
	if (DefaultAttributes)
		ApplyEffectToSelf(DefaultAttributes, 1.f);
}

void ADAAICharacter_NPC::BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute,
	UDaBaseAttributeSet* AttributeSet) const
{
	check(AttributeInfo);
	
	FDaAttributeData AttributeData = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	AttributeData.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	OnNPCAttributeChanged.Broadcast(AttributeData);
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

