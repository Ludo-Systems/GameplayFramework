// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Actor.h"
#include "DaInteractableInterface.h"
#include "DaPickupItem.generated.h"

class UDaAbilitySystemComponent;
class USphereComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActiveStateChanged, AActor*, InstigatorActor, bool, IsActive);

UCLASS()
class GAMEPLAYFRAMEWORK_API ADaPickupItem : public AActor, public IDaInteractableInterface, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADaPickupItem();

	// Interactable interface
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
	virtual FText GetInteractText_Implementation(APawn* InstigatorPawn) override;
	virtual void HighlightActor_Implementation() override;
	virtual void UnHighlightActor_Implementation() override;

	UFUNCTION(BlueprintCallable, Category="PickupItem")
	void HideAndCooldownItem(APawn* InstigatorPawn);
	
	UPROPERTY(BlueprintAssignable, Category="PickupItem")
	FOnActiveStateChanged OnActiveStateChanged;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
protected:

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Components")
	TObjectPtr<USphereComponent> SphereComp;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Components")
	TObjectPtr<UStaticMeshComponent> BaseMeshComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
	TObjectPtr<UDaAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Respawn Settings")
	bool bShouldRespawn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Respawn Settings")
	bool bShouldRespawnOnDeath;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Respawn Settings")
	float RespawnDelay;

	// Enabled by default. Disable if not using the default hitflash material function so we don't try to set material params when hit
	UPROPERTY(EditAnywhere, Category="Effects")
	bool bUseDefaultHitFlash;
	
	// Parameter to trigger flash effect for MF_HitFlash Material Function
	UPROPERTY(VisibleAnywhere, Category="Effects")
	FName HitFlashTimeParamName;

	// Parameter to set color for flash effect for MF_HitFlash Material Function
	UPROPERTY(VisibleAnywhere, Category="Effects")
	FName HitFlashColorParamName;

	UPROPERTY(EditAnywhere, Category="Effects")
	FVector HitFlashColor;
	
	UPROPERTY(Replicated)
	bool bIsActive;

	UFUNCTION()
	void OnPlayerRespawn(APawn* OldPawn, APawn* NewPawn);
	
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastActiveStateChanged(AActor* InstigatorActor, bool NewState);
	
	UFUNCTION()
	void FadeMesh(AActor* InstigatorActor);

	UFUNCTION()
	void ShowItem(APawn* InstigatorPawn);
	
	void SetItemState(APawn* InstigatorPawn, bool bNewIsActive);

	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly)
	bool bHighlighted = false;
};
