// Fill out your copyright notice in the Description page of Project Settings.


#include "DaCharacterBase.h"

#include "CoreGameplayTags.h"
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

	bUseDefaultHitFlash = true;
	HitFlashTimeParamName = "HitFlashTime";
	HitFlashColorParamName = "HitFlashColor";
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


