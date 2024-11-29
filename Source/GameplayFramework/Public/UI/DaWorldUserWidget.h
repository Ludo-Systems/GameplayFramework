// Copyright Dream Awake Solutions LLC

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DaWorldUserWidget.generated.h"

class USizeBox;
/**
 * 
 */
UCLASS()
class GAMEPLAYFRAMEWORK_API UDaWorldUserWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USizeBox> ParentSizeBox;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	UPROPERTY(BlueprintReadWrite, Category="DA|UI", meta = (ExposeOnSpawn=true))
	TObjectPtr<AActor> AttachedActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DA|UI", meta = (ExposeOnSpawn=true))
	FVector WorldOffset;
};
