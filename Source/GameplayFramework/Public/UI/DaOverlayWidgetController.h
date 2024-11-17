// Copyright Dream Awake Solutions LLC

#pragma once

#include "CoreMinimal.h"
#include "DaWidgetController.h"
#include "GameplayTagContainer.h"
#include "DaOverlayWidgetController.generated.h"


USTRUCT(BlueprintType)
struct FUIWidgetRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag MessageTag = FGameplayTag();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Message = FText();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UDaUserWidgetBase> MessageWidget;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* Image = nullptr;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxHealthChangedSignature, float, NewMaxHealth);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnManaChangedSignature, float, NewMana);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxManaChangedSignature, float, NewMaxMana);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRowSignature, FUIWidgetRow, Row);


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

	UPROPERTY(BlueprintAssignable, Category="UI Messages")
	FMessageWidgetRowSignature MessageWidgetRowDelegate;
	
protected:

	// TODO: Replace with Data Asset and load with asset manager
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UI Data")
	TObjectPtr<UDataTable> MessageWidgetDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UI Data")
	FName MessageParentTag = "Message";
	
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

	template<typename T>
	T* GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag);
	
};

template <typename T>
T* UDaOverlayWidgetController::GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag)
{
	return DataTable->FindRow<T>(Tag.GetTagName(), TEXT(""));
}
