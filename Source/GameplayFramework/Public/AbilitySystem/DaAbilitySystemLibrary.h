// Copyright Dream Awake Solutions LLC

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DaAbilitySystemLibrary.generated.h"

class UDaOverlayWidgetController;
/**
 * 
 */
UCLASS()
class GAMEPLAYFRAMEWORK_API UDaAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category="DaAbilitySystemLibrary|WidgetController")
	static UDaOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);
};
