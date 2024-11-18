// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DaCharacterBase.h"
#include "DaCharacter.generated.h"

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
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UDaInteractionComponent> InteractionComp;
	
	virtual void InitAbilitySystem() override;

	// Override to disable input on player controller, still calls super to handle death
	virtual void OnDeathStarted(AActor* OwningActor, AActor* InstigatorActor) override;
	
};
