// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/DaPanelWidget.h"
#include "DaAppraisalWidget.generated.h"

class UCECollectibleViewModel;
/**
 * 
 */
UCLASS()
class COLLECTIBLES_API UDaAppraisalWidget : public UDaPanelWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UCECollectibleViewModel> CollectibleViewModel;
	
};
