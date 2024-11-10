// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Character.h"
#include "DaCharacterBase.generated.h"

class UDaAbilitySystemComponent;
class UDaAttributeComponent;
class UDaWorldUserWidget;

UCLASS()
class GAMEPLAYFRAMEWORK_API ADaCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADaCharacterBase();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void InitAbilitySystem();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDaAttributeComponent> AttributeComp;

	// Player Characters will get this from Player State, NPC subclasses *MUST* create it in their constructors
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDaAbilitySystemComponent> AbilitySystemComponent;

	// Calls InitAbilitySystem for setup of player characters on server and client
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	// Calls InitAbilitySystem for setup of non-player characters (like AI NPCs) on server only
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnHealthChanged(UDaAttributeComponent* HealthComponent, float OldHealth, float NewHealth, AActor* InstigatorActor);
	
	UFUNCTION()
	virtual void OnDeathStarted(AActor* OwningActor, AActor* InstigatorActor);
	
	UFUNCTION()
	virtual void OnDeathFinished(AActor* OwningActor);
	
	/* Can be used to identify the type of character. Defaults to Character.Type, but subclass such as AI or Player bases classes can set Character.Type.AI, or Character.Type.Player,
	 * subclasses could get more specific like Character.Type.Player.Spaceship or Character.Type.AI.Enemy but these tags will need to be defined as needed.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="DA|GameplayTags")
	FGameplayTag CharacterTypeGameplayTag;

	/* Can be used to identify a specific avatar actor character (and possibly what skin they are using) like Character.ID.Gideon.Inquisitor. Will be set to a default Character.ID tag if unspecified. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="DA|GameplayTags")
	FGameplayTag CharacterIDGameplayTag;

	UPROPERTY(VisibleDefaultsOnly, Category="DA|UI")
	TObjectPtr<UDaWorldUserWidget> ActiveHealthBar;

	UPROPERTY(EditDefaultsOnly, Category="DA|UI")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category="DA|UI")
	TSubclassOf<UUserWidget> DamagePopUpWidgetClass;
	
	// Enabled by default. Disable if characters are not using the default hitflash material function so we don't try to set material params when hit
	UPROPERTY(EditAnywhere, Category="DA|Effects")
	bool bUseDefaultHitFlash;
	
	// Parameter to trigger flash effect for MF_HitFlash Material Function
	UPROPERTY(VisibleAnywhere, Category="DA|Effects")
	FName HitFlashTimeParamName;

	// Parameter to set color for flash effect for MF_HitFlash Material Function
	UPROPERTY(VisibleAnywhere, Category="DA|Effects")
	FName HitFlashColorParamName;
	
	// Subclasses can create and install the ActiveHealthBar as needed, usually after taking damage the first time
	UFUNCTION(BlueprintCallable)
	void ShowSetHealthBarWidget();

	UFUNCTION(BlueprintCallable)
	void ShowDamagePopupWidget(float Damage);
};
