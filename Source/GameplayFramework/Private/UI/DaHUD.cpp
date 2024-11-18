// Copyright Dream Awake Solutions LLC


#include "UI/DaHUD.h"

#include "UI/DaOverlayWidgetController.h"
#include "UI/DaStatMenuWidgetController.h"
#include "UI/DaUserWidgetBase.h"

UDaOverlayWidgetController* ADaHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UDaOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		OverlayWidgetController->BindCallbacksToDependencies();
	}
	return OverlayWidgetController;
}

UDaStatMenuWidgetController* ADaHUD::GetStatMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	if (StatMenuWidgetController == nullptr)
	{
		StatMenuWidgetController = NewObject<UDaStatMenuWidgetController>(this, StatMenuWidgetControllerClass);
		StatMenuWidgetController->SetWidgetControllerParams(WCParams);
		StatMenuWidgetController->BindCallbacksToDependencies();
	}
	return StatMenuWidgetController;
}

void ADaHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UDaBaseAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("UDaOverlayWidgetClass uninitialized, fill out DaHud."));
	checkf(OverlayWidgetControllerClass, TEXT("UDaOverlayWidgetController Class uninitialized, fill out DaHud."));

	UUserWidget* Widget = CreateWidget<UUserWidget>(PC, OverlayWidgetClass);
	OverlayWidget = Cast<UDaUserWidgetBase>(Widget);

	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	UDaOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);

	OverlayWidget->SetWidgetController(WidgetController);
	WidgetController->BroadcastInitialValues();
	
	Widget->AddToViewport();
}

void ADaHUD::RemoveOverlay()
{
	if (OverlayWidget)
	{
		OverlayWidget->RemoveFromParent();
		OverlayWidget->Destruct();
	}

	if (OverlayWidgetController)
	{
		OverlayWidgetController->MarkAsGarbage();
		OverlayWidgetController = nullptr;
	}
}
