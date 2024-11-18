// Copyright Dream Awake Solutions LLC

#pragma once

#include "CoreMinimal.h"
#include "DaWidgetController.h"
#include "GameplayTagContainer.h"
#include "DaStatMenuWidgetController.generated.h"

struct FDaAttributeData;
class UDaAttributeInfo;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoSignature, const FDaAttributeData&, Info);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class GAMEPLAYFRAMEWORK_API UDaStatMenuWidgetController : public UDaWidgetController
{
	GENERATED_BODY()

public:

	virtual void BindCallbacksToDependencies() override;
	virtual void BroadcastInitialValues() override;

	UPROPERTY(BlueprintAssignable, Category="Attributes")
	FAttributeInfoSignature AttributeInfoDelegate;
	
protected:
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UDaAttributeInfo> AttributeInfo;

private:

	void BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const;
	
};
