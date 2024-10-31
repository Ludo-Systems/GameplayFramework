// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DaUserWidgetBase.h"

void UDaUserWidgetBase::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}
