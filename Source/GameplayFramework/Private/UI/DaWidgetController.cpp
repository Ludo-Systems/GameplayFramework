// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DaWidgetController.h"

void UDaWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeComponent = WCParams.AttributeComponent;
}

void UDaWidgetController::BroadcastInitialValues()
{
}

void UDaWidgetController::BindCallbacksToDependencies()
{
}
