// Fill out your copyright notice in the Description page of Project Settings.


#include "DaProjectile_Magic.h"

#include "CoreGameplayTags.h"
#include "Abilities/Async/AbilityAsync_WaitGameplayEvent.h"
#include "AbilitySystem/DaAbilitySystemComponent.h"
#include "Components/SphereComponent.h"
#include "DaCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"

ADaProjectile_Magic::ADaProjectile_Magic()
{
	DamageAmount = 20.0f;
	MovementComp->InitialSpeed = 8000.0f;
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ADaProjectile_Magic::OnActorOverlap);
	InitialLifeSpan = 10.0f;

	bReplicates = true;
}

void ADaProjectile_Magic::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                         UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AActor* InstigatorActor = GetInstigator();
	if (HasAuthority() && OtherActor && OtherActor != InstigatorActor)
	{
		UAbilitySystemComponent* InstigatorASC =  nullptr;
		if (Cast<IAbilitySystemInterface>(InstigatorActor))
		{
			InstigatorASC =  CastChecked<IAbilitySystemInterface>(InstigatorActor)->GetAbilitySystemComponent();
		}
		
		UAbilitySystemComponent* OtherActorASC = nullptr;
		if (Cast<IAbilitySystemInterface>(OtherActor))
		{
			OtherActorASC =  Cast<IAbilitySystemInterface>(OtherActor)->GetAbilitySystemComponent();
		}
		
		// Parry
		if (InstigatorASC && OtherActorASC)
		{
			if (OtherActorASC->HasMatchingGameplayTag(CoreGameplayTags::TAG_Status_Parrying))
			{
				// parry
				// Movement component has bRotationFollowsVelocity set to true, so we can simply invert velocity
				// Set New instigator so our check above wont fail after Parry is complete
				MovementComp->Velocity = -MovementComp->Velocity;
				SetInstigator(Cast<APawn>(OtherActor));
				return;
			}

			if (ensure(DamageEffect))
			{
				// deal damage
				FGameplayEffectContextHandle EffectContext = InstigatorASC->MakeEffectContext();
				EffectContext.AddSourceObject(this);
				EffectContext.AddInstigator(InstigatorActor, this);
				EffectContext.AddHitResult(SweepResult);
				
				FGameplayEffectSpecHandle NewHandle = InstigatorASC->MakeOutgoingSpec(DamageEffect, 0, EffectContext);
				if (NewHandle.IsValid())
				{
					NewHandle.Data.Get()->SetSetByCallerMagnitude(CoreGameplayTags::TAG_Gameplay_Damage, DamageAmount);
					FActiveGameplayEffectHandle ActiveGEHandle = InstigatorASC->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), OtherActorASC);
				}

				// Create a damage event and send to both instigator and damaged actor in case they have any triggered abilities
				FGameplayEventData EventData = FGameplayEventData();
				EventData.Instigator = InstigatorActor;
				EventData.Target = OtherActor;
				EventData.EventTag = CoreGameplayTags::TAG_Event_Damage;
				EventData.OptionalObject = SweepResult.GetComponent();
				EventData.EventMagnitude = DamageAmount;
				
				InstigatorASC->HandleGameplayEvent(CoreGameplayTags::TAG_Event_Damage, &EventData);
				OtherActorASC->HandleGameplayEvent(CoreGameplayTags::TAG_Event_Damage, &EventData);
			}
			
			Explode();
		}
	}
}
