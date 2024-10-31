// Copyright Dream Awake Solutions LLC

#pragma once

#include "CoreMinimal.h"
#include "UI/DaWorldUserWidget.h"
#include "DaDamageWidget.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPLAYFRAMEWORK_API UDaDamageWidget : public UDaWorldUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, Category="DA|UI", meta = (ExposeOnSpawn=true))
	float Damage;
};
