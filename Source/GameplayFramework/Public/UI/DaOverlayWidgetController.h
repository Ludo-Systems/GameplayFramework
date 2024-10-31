// Copyright Dream Awake Solutions LLC

#pragma once

#include "CoreMinimal.h"
#include "DaWidgetController.h"
#include "DaOverlayWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxHealthChangedSignature, float, NewMaxHealth);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnManaChangedSignature, float, NewMana);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxManaChangedSignature, float, NewMaxMana);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class GAMEPLAYFRAMEWORK_API UDaOverlayWidgetController : public UDaWidgetController
{
	GENERATED_BODY()
public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;
	
	UPROPERTY(BlueprintAssignable, Category="Character Attributes")
	FOnHealthChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category="Character Attributes")
	FOnMaxHealthChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Category="Character Attributes")
	FOnManaChangedSignature OnManaChanged;

	UPROPERTY(BlueprintAssignable, Category="Character Attributes")
	FOnMaxManaChangedSignature OnMaxManaChanged;

protected:

	UFUNCTION()
	void HealthChanged(UDaAttributeComponent* HealthComponent, float OldHealth, float NewHealth,
	AActor* InstigatorActor);

	UFUNCTION()
	void MaxHealthChanged(UDaAttributeComponent* HealthComponent, float OldMaxHealth, float NewMaxHealth,
	AActor* InstigatorActor);

	UFUNCTION()
	void ManaChanged(UDaAttributeComponent* HealthComponent, float OldMana, float NewMana,
	AActor* InstigatorActor);

	UFUNCTION()
	void MaxManaChanged(UDaAttributeComponent* HealthComponent, float OldMaxMana, float NewMaxMana,
	AActor* InstigatorActor);
	
};
