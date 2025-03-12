// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AttributeSet.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Character.h"
#include "DAAICharacter_NPC.generated.h"

class UDaAttributeInfo;
class UDaBaseAttributeSet;
class UDaPawnData;
class UGameplayEffect;
class UDaAbilitySystemComponent;
class UDaNPCDialogData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDANPC_AttributeInfoSignature, const FDaAttributeData&, Info);

UCLASS()
class GAMEPLAYFRAMEWORK_API ADAAICharacter_NPC : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADAAICharacter_NPC();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void InitAbilitySystem();

	// Fires off the DefaultPrimaryAttributes GameplayEffect which should be setup to initialize any attributes passed in to the character's pawn data AbilitySet
	void ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& GameplayEffectClass, const float Level) const;
	void InitDefaultAttributes() const;
	void BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute, UDaBaseAttributeSet* AttributeSet) const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Player Characters will get this from Player State, NPC subclasses *MUST* create it in their constructors
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UDaAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintAssignable, Category="Attributes")
	FDANPC_AttributeInfoSignature OnNPCAttributeChanged;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AI")
	FGameplayTagContainer AttributeSetTags;

	UPROPERTY(EditDefaultsOnly, Category="AI")
	TObjectPtr<UDaAttributeInfo> AttributeInfo;

	UFUNCTION(BlueprintCallable)
	void UpgradeAttribute(const FGameplayTag& AttributeTag, int32 Amount);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AI")
	TObjectPtr<UDaPawnData> PawnData;
	
	// Gameplay effect used to set all (non-vital) attributes (provided in an AbilitySet) to default.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AI")
	TSubclassOf<UGameplayEffect> DefaultAttributes;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AI")
	FGameplayTag CharacterTypeGameplayTag;

	/* Can be used to identify a specific avatar actor character (and possibly what skin they are using) like Character.ID.Gideon.Inquisitor. Will be set to a default Character.ID tag if unspecified. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AI")
	FGameplayTag CharacterIDGameplayTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	TSubclassOf<UDaNPCDialogData> NPCDialogDataClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AI")
	TObjectPtr<UDaNPCDialogData> NPCDialogData;
};
