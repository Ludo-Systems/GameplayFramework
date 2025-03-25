// Copyright Dream Awake Solutions LLC

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "DaWidgetController.generated.h"

class UDaAbilitySystemComponent;
class UDaAttributeInfo;
class UDaBaseAttributeSet;
class UAttributeSet;

// Use to broadcast a full attribute info struct for a given attribute. Note AttributeInfo DataAsset must contain an entry for the attribute for this to fire
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoSignature, const FDaAttributeData&, Info);

USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()

	FWidgetControllerParams() {}
	FWidgetControllerParams(APlayerController* PC, APlayerState* PS, UDaAbilitySystemComponent* ASC, const FGameplayTagContainer& AsTags)
		: PlayerController(PC), PlayerState(PS), AbilitySystemComponent(ASC), AttributeSetTags(AsTags) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerController> PlayerController = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerState> PlayerState = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UDaAbilitySystemComponent> AbilitySystemComponent = nullptr;

	UPROPERTY (EditAnywhere, BlueprintReadWrite)
	FGameplayTagContainer AttributeSetTags;
};

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class GAMEPLAYFRAMEWORK_API UDaWidgetController : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerParams(const FWidgetControllerParams& WCParams);

	UFUNCTION(BlueprintCallable)
	virtual void BroadcastInitialValues();

	UFUNCTION(BlueprintCallable)
	virtual void BindCallbacksToDependencies();
	
	UPROPERTY(BlueprintAssignable, Category="Attributes")
	FAttributeInfoSignature AttributeInfoDelegate;
	
protected:

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<APlayerState> PlayerState;

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<UDaAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	FGameplayTagContainer AttributeSetTags;

	UPROPERTY(EditDefaultsOnly, Category="Attributes")
	TObjectPtr<UDaAttributeInfo> AttributeInfo;

	
	virtual void BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute, UDaBaseAttributeSet* AttributeSet) const;
	
};
