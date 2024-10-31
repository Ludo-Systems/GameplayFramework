// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DaInteractionComponent.generated.h"

class UDaWorldUserWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAMEPLAYFRAMEWORK_API UDaInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Returns the health component if one exists on the specified actor.
	UFUNCTION(BlueprintPure, Category = "DA|Interaction")
	static UDaInteractionComponent* FindInteractionComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UDaInteractionComponent>() : nullptr); }
	
	UFUNCTION(BlueprintCallable, Category = "DA|Interaction")
	void PrimaryInteract();
	
	UDaInteractionComponent();

protected:
	UFUNCTION(Server, Reliable)
	void ServerInteract(AActor* InFocus);

	void FindBestInteractable();
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY()
	TObjectPtr<AActor> FocusedActor;
	
	UPROPERTY(EditDefaultsOnly, Category="DA|Trace")
	float TraceDistance;

	UPROPERTY(EditDefaultsOnly, Category="DA|Trace")
	float TraceRadius;

	UPROPERTY(EditDefaultsOnly, Category="DA|Trace")
	TEnumAsByte<ECollisionChannel> CollisionChannel;

	UPROPERTY(EditDefaultsOnly, Category="DA|UI")
	TSubclassOf<UDaWorldUserWidget> DefaultWidgetClass;

	UPROPERTY()
	TObjectPtr<UDaWorldUserWidget> DefaultWidgetInstance;

	void RemoveWidget();
	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
