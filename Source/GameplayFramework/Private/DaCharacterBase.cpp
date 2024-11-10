// Fill out your copyright notice in the Description page of Project Settings.


#include "DaCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "CoreGameplayTags.h"
#include "DaAttributeComponent.h"
#include "GameplayFramework.h"
#include "Blueprint/UserWidget.h"
#include "UI/DaDamageWidget.h"
#include "UI/DaWorldUserWidget.h"
#include "WorldPartition/ContentBundle/ContentBundleLog.h"


// Sets default values
ADaCharacterBase::ADaCharacterBase()
{
	CharacterIDGameplayTag = CoreGameplayTags::TAG_Character_ID;
	CharacterTypeGameplayTag = CoreGameplayTags::TAG_Character_Type;

	AttributeComp = CreateDefaultSubobject<UDaAttributeComponent>("AttributeComp");

	GetMesh()->SetGenerateOverlapEvents(true);
	
	bUseDefaultHitFlash = true;
	HitFlashTimeParamName = "HitFlashTime";
	HitFlashColorParamName = "HitFlashColor";
}

UAbilitySystemComponent* ADaCharacterBase::GetAbilitySystemComponent() const
{
	return Cast<UAbilitySystemComponent>(AbilitySystemComponent);
}

void ADaCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	if (CharacterIDGameplayTag.MatchesAny(FGameplayTagContainer(CoreGameplayTags::TAG_Character_Type_AI)))
	{
		InitAbilitySystem();
	}
}

void ADaCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (CharacterIDGameplayTag.MatchesAny(FGameplayTagContainer(CoreGameplayTags::TAG_Character_Type_Player)))
	{
		//server
		InitAbilitySystem();
	}

}

void ADaCharacterBase::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	if (CharacterIDGameplayTag.MatchesAny(FGameplayTagContainer(CoreGameplayTags::TAG_Character_Type_Player)))
	{
		//client
		InitAbilitySystem();
	}
}

void ADaCharacterBase::InitAbilitySystem()
{
	// must be implemented by sub classes
	checkf(0, TEXT("ADaCharacterBase::InitAbilitySystem must be implemented by sub classes. Do not call super"));
}

void ADaCharacterBase::ShowSetHealthBarWidget()
{
	if (HealthBarWidgetClass && ActiveHealthBar == nullptr)
	{
		ActiveHealthBar = CreateWidget<UDaWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
		if (ActiveHealthBar)
		{
			ActiveHealthBar->AttachedActor = this;
			ActiveHealthBar->AddToViewport();
		}
	}
}

void ADaCharacterBase::ShowDamagePopupWidget(float Damage)
{
	if (DamagePopUpWidgetClass)
	{
		UDaDamageWidget* DamagePopUpWidget = CreateWidget<UDaDamageWidget>(GetWorld(), DamagePopUpWidgetClass);
		//LOG("Damage popup widget: %s", *GetNameSafe(DamagePopUpWidget));
		if (DamagePopUpWidget)
		{
			DamagePopUpWidget->AttachedActor = this;
			DamagePopUpWidget->Damage = Damage;
			DamagePopUpWidget->AddToViewport();
		}
	}
}


