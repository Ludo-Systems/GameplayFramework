// Copyright Dream Awake Solutions LLC

#pragma once

#include "CoreMinimal.h"
#include "DaAttributeComponent.h"
#include "GameFramework/HUD.h"
#include "DaHUD.generated.h"

class UDaStatMenuWidgetController;
class UDaBaseAttributeSet;
class UDaAttributeComponent;
class UAbilitySystemComponent;
class UDaOverlayWidgetController;
class UDaUserWidgetBase;
struct FWidgetControllerParams;

/**
 * 
 */
UCLASS()
class GAMEPLAYFRAMEWORK_API ADaHUD : public AHUD
{
	GENERATED_BODY()

public:

	UDaOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);
	
	UDaStatMenuWidgetController* GetStatMenuWidgetController(const FWidgetControllerParams& WCParams);
	
	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UDaBaseAttributeSet* AS);

	void RemoveOverlay();
	
private:

	UPROPERTY()
	TObjectPtr<UDaUserWidgetBase> OverlayWidget;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UDaUserWidgetBase> OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<UDaOverlayWidgetController> OverlayWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UDaOverlayWidgetController> OverlayWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UDaStatMenuWidgetController> StatMenuWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UDaStatMenuWidgetController> StatMenuWidgetControllerClass;
	
};
