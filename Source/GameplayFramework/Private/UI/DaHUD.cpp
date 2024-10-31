// Copyright Dream Awake Solutions LLC


#include "UI/DaHUD.h"

#include "UI/DaOverlayWidgetController.h"
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

void ADaHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UDaAttributeComponent* AC)
{
	checkf(OverlayWidgetClass, TEXT("UDaOverlayWidgetClass uninitialized, fill out DaHud."));
	checkf(OverlayWidgetControllerClass, TEXT("UDaOverlayWidgetController Class uninitialized, fill out DaHud."));

	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UDaUserWidgetBase>(Widget);

	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AC);
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
