// Copyright Dream Awake Solutions LLC

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/HUD.h"
#include "DaHUD.generated.h"

class UDaAbilitySystemComponent;
class UDaStatMenuWidgetController;
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
	
	void InitOverlay(APlayerController* PC, APlayerState* PS, UDaAbilitySystemComponent* ASC);

	void RemoveOverlay();

	FORCEINLINE FGameplayTagContainer GetOverlayAttributeSetTags() { return OverlayWidgetAttributeSetTags; } 
	FORCEINLINE FGameplayTagContainer GetStatMenuAttributeSetTags() { return StatMenuWidgetAttributeSetTags; } 

protected:

	UPROPERTY(EditAnywhere, Category=UI)
	TSubclassOf<UDaUserWidgetBase> OverlayWidgetClass;
	
	UPROPERTY(EditAnywhere, Category=UI)
	TSubclassOf<UDaOverlayWidgetController> OverlayWidgetControllerClass;
	
	UPROPERTY(EditAnywhere, Category=UI)
	FGameplayTagContainer OverlayWidgetAttributeSetTags;
	
	UPROPERTY(EditAnywhere, Category=UI)
	TSubclassOf<UDaStatMenuWidgetController> StatMenuWidgetControllerClass;

	UPROPERTY(EditAnywhere, Category=UI)
	FGameplayTagContainer StatMenuWidgetAttributeSetTags;
	
private:

	UPROPERTY()
	TObjectPtr<UDaUserWidgetBase> OverlayWidget;

	UPROPERTY()
	TObjectPtr<UDaOverlayWidgetController> OverlayWidgetController;

	UPROPERTY()
	TObjectPtr<UDaStatMenuWidgetController> StatMenuWidgetController;
	
};
