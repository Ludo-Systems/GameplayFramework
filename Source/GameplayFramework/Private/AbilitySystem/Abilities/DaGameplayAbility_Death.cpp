// Copyright Dream Awake Solutions LLC


#include "AbilitySystem/Abilities/DaGameplayAbility_Death.h"

#include "AbilitySystem/DaAbilitySystemComponent.h"
#include "DaAttributeComponent.h"
#include "CoreGameplayTags.h"

UDaGameplayAbility_Death::UDaGameplayAbility_Death(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;

	bAutoStartDeath = true;

	if (HasAnyFlags(RF_ClassDefaultObject))
	{
		// Add the ability trigger tag as default to the CDO.
		FAbilityTriggerData TriggerData;
		TriggerData.TriggerTag = CoreGameplayTags::TAG_Event_Death;
		TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
		AbilityTriggers.Add(TriggerData);
	}
}

void UDaGameplayAbility_Death::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{			
		constexpr bool bReplicateEndAbility = true;
		constexpr bool bWasCancelled = true;
		EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	}
	
	check(ActorInfo);
	UDaAbilitySystemComponent* ASC = CastChecked<UDaAbilitySystemComponent>(ActorInfo->AbilitySystemComponent.Get());

	FGameplayTagContainer AbilityTypesToIgnore;
	AbilityTypesToIgnore.AddTag(CoreGameplayTags::Ability_Behavior_SurvivesDeath);

	// TODO: Figure out why cancelling doesnt block Actions, for now, just block
	//FGameplayTagContainer AbilityTypesToCancel;
	//AbilityTypesToCancel.AddTag(SharedGameplayTags::TAG_Action);
	
	// Cancel all abilities and block others from starting unless they have the ignore tag.
	ASC->CancelAbilities(nullptr, &AbilityTypesToIgnore, this);
	SetCanBeCanceled(false);

	FGameplayTagContainer AbilityTypesToBlock;
	AbilityTypesToBlock.AddTag(CoreGameplayTags::TAG_Action);
	ASC->BlockAbilitiesWithTags(AbilityTypesToBlock);
	
	if (bAutoStartDeath)
	{
		if (UDaAttributeComponent* HealthComponent = UDaAttributeComponent::FindAttributeComponent(GetAvatarActorFromActorInfo()))
		{
			if (StartDeath(HealthComponent, ConstCast(TriggerEventData->Instigator)))
			{
				FTimerHandle TimerHandle_EndAbility;
				constexpr float EndDelay = 5.0f;
				FTimerDelegate Delegate;
				Delegate.BindUObject(this, &ThisClass::FinishDeathAndEndAbility, HealthComponent, Handle, ActorInfo, ActivationInfo, true, false);
				GetWorld()->GetTimerManager().SetTimer(TimerHandle_EndAbility, Delegate, EndDelay, false);
			}
			else
			{
				constexpr bool bReplicateEndAbility = true;
				constexpr bool bWasCancelled = true;
				EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
			}
		}
		else
		{
			constexpr bool bReplicateEndAbility = true;
			constexpr bool bWasCancelled = true;
			EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
		}
	}
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UDaGameplayAbility_Death::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	check(ActorInfo);

	// Always try to finish the death when the ability ends in case the ability doesn't.
	// This won't do anything if the death hasn't been started.
	FinishDeath();
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool UDaGameplayAbility_Death::StartDeath(UDaAttributeComponent* HealthComponent, AActor* InstigatorActor)
{
	if (HealthComponent->GetDeathState() == EDaDeathState::NotDead)
	{
		return HealthComponent->StartDeath(InstigatorActor);
	}
	
	return false;
}

void UDaGameplayAbility_Death::FinishDeath()
{
	if (UDaAttributeComponent* HealthComponent = UDaAttributeComponent::FindAttributeComponent(GetAvatarActorFromActorInfo()))
	{
		if (HealthComponent->GetDeathState() == EDaDeathState::DeathStarted)
		{
			HealthComponent->FinishDeath();
		}
	}
}

void UDaGameplayAbility_Death::FinishDeathAndEndAbility(UDaAttributeComponent* HealthComponent, 
	const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (HealthComponent->GetDeathState() == EDaDeathState::DeathStarted)
	{
		HealthComponent->FinishDeath();

		// this will call FinishDeath but theoretically it wont do anything since we've already finished. 
		EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	}
}
