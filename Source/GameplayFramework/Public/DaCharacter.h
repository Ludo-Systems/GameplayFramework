// Copyright Dream Awake Solutions LLC

#pragma once

#include "CoreMinimal.h"
#include "DaCharacterBase.h"
#include "DaCharacter.generated.h"

class ADaInspectableItem;
class UDaInteractionComponent;
class UDaAttributeComponent;

UCLASS()
class GAMEPLAYFRAMEWORK_API ADaCharacter : public ADaCharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADaCharacter();

	// Interaction Component 
	void PrimaryInteraction() const;

	void InitPlayerHUD() const;

	virtual int32 GetCharacterLevel() override;

	UFUNCTION(BlueprintCallable)
	void SetInspectedItem(AActor* Item);

	UFUNCTION(BlueprintCallable)
	void SetMovingItem(AActor* Item);
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UDaInteractionComponent> InteractionComp;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<AActor> InspectedItem;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<AActor> MovingItem;
	
	virtual void InitAbilitySystem() override;

	// Override to disable input on player controller, still calls super to handle death
	virtual void OnDeathStarted(AActor* OwningActor, AActor* InstigatorActor) override;

	UFUNCTION()
	void InspectedItemStateChanged(AActor* Item, AActor* InspectingActor, bool IsInspecting);
	
};
