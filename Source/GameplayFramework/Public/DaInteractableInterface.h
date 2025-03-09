// Copyright Dream Awake Solutions LLC

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DaInteractableInterface.generated.h"

class UDaWorldUserWidget;
// This class does not need to be modified.
UINTERFACE(Blueprintable)
class GAMEPLAYFRAMEWORK_API UDaInteractableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GAMEPLAYFRAMEWORK_API IDaInteractableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, Category = "Interactable")
	void OnActorLoaded();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interactable")
	void Interact(APawn* InstigatorPawn);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interactable")
	void SecondaryInteract(APawn* InstigatorPawn);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interactable")
	FText GetInteractText(APawn* InstigatorPawn);

	UFUNCTION(BlueprintNativeEvent, Category = "Interactable")
	TSubclassOf<UDaWorldUserWidget> GetInteractWidget();
	
	UFUNCTION(BlueprintNativeEvent, Category = "Interactable")
	void HighlightActor();

	UFUNCTION(BlueprintNativeEvent, Category = "Interactable")
	void UnHighlightActor();
};
