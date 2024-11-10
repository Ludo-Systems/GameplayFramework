// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DaCharacterBase.h"
#include "DaAICharacter.generated.h"

class UDaAbilitySystemComponent;
class UDaAttributeComponent;
class UPawnSensingComponent;
class UUserWidget;
class UDaWorldUserWidget;

UCLASS()
class GAMEPLAYFRAMEWORK_API ADaAICharacter : public ADaCharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADaAICharacter();
	
	virtual void InitAbilitySystem() override;
	
protected:

	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<UPawnSensingComponent> PawnSensingComp;

	UPROPERTY(VisibleDefaultsOnly, Category="UI")
	TObjectPtr<UDaWorldUserWidget> PlayerSeenWidget;
	
	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<UUserWidget> PlayerSeenWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category="UI")
	float PlayerSeenEmoteTime;
	
	UPROPERTY(VisibleDefaultsOnly, Category="AI")
	FName TargetActorKey;

	// Health attribute set used by this actor.
	UPROPERTY()
	TObjectPtr<const class UDaCharacterAttributeSet> HealthSet;
	// Combat attribute set used by this actor.
	UPROPERTY()
	TObjectPtr<const class UDaCombatAttributeSet> CombatSet;
	
	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastOnPawnSeen(APawn* Pawn);

	AActor* GetTargetActor();
	void SetTargetActor(AActor* NewTarget);

	void PlayerSeenWidgetTimeExpired();
	
	virtual void PostInitializeComponents() override;

	//virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnHealthChanged(UDaAttributeComponent* HealthComponent, float OldHealth, float NewHealth, AActor* InstigatorActor);

	UFUNCTION()
	void OnDeathStarted(AActor* OwningActor, AActor* InstigatorActor);

	UFUNCTION()
	void OnDeathFinished(AActor* OwningActor);

};
