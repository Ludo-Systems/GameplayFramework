// Copyright Dream Awake Solutions LLC

#pragma once

#include "CoreMinimal.h"
#include "DaPlayerController.h"
#include "DaPlayerController_TopDown.generated.h"

class USplineComponent;
/**
 * 
 */
UCLASS()
class GAMEPLAYFRAMEWORK_API ADaPlayerController_TopDown : public ADaPlayerController
{
	GENERATED_BODY()

public:
	ADaPlayerController_TopDown();
	
	virtual void AbilityInputTagPressed(const FInputActionValue& Value, FGameplayTag InputTag) override;
	virtual void AbilityInputTagHeld(FGameplayTag InputTag) override;
	virtual void AbilityInputTagReleased(FGameplayTag InputTag) override;

	virtual void PlayerTick(float DeltaTime) override;

	virtual void SpawnClickFX();
	
protected:
	
	/** Time Threshold to know if it was a short press */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
	float ShortPressThreshold = 0.5f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
	float AutoRunAcceptanceRadius = 50.f;

	UPROPERTY(VisibleAnywhere, Category = Input)
	TObjectPtr<USplineComponent> Spline;

	FVector CachedDestination = FVector::ZeroVector;
	float FollowedTime = 0.f;
	bool bAutoRunning = false;
	bool bTargeting = false;
	
	void AutoRun();
	
};
