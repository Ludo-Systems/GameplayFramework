// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DaTaggedWidget.h"

#include "DaPlayerState.h"
#include "AbilitySystem/DaAbilitySystemComponent.h"

UDaTaggedWidget::UDaTaggedWidget(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
}

void UDaTaggedWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!IsDesignTime())
	{
		// Listen for tag changes on our hidden tags
		if (ADaPlayerState* PlayerState = Cast<ADaPlayerState>(GetOwningPlayerState()))
		{
			UDaAbilitySystemComponent* ASC = PlayerState->GetDaAbilitySystemComponent();
			GameplayTagPropertyMap.Initialize(this, ASC);
		}
	}
}
