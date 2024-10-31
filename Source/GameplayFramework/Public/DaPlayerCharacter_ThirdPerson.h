// Copyright Dream Awake Solutions LLC

#pragma once

#include "CoreMinimal.h"
#include "DaCharacter.h"
#include "DaPlayerCharacter_ThirdPerson.generated.h"

struct FInputActionValue;
class UDaInteractionComponent;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class GAMEPLAYFRAMEWORK_API ADaPlayerCharacter_ThirdPerson : public ADaCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADaPlayerCharacter_ThirdPerson();

	// Override GetPawnViewLocation for ThirdPerson since this is setup by default for first person
	virtual FVector GetPawnViewLocation() const override;

	void PrimaryInteraction() const;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<USpringArmComponent> SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UCameraComponent> CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UDaInteractionComponent> InteractionComp;

	void FindCrosshairTarget();

	void CrosshairTraceComplete(const FTraceHandle& InTraceHandle, FTraceDatum& InTraceDatum);

	FTraceHandle TraceHandle;

	virtual void Tick(float DeltaTime) override;

	void LookStick(const FInputActionValue& InputValue);

private:

	bool bHasPawnTarget;
	
};
