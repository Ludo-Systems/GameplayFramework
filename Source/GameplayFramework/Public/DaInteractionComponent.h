// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DaInteractionComponent.generated.h"

class UDaWorldUserWidget;

UENUM()
enum class EInteractionType : uint8
{
	NoInteraction,
	SphereTrace,
	MouseCursor
};

UENUM()
enum class EInteractionHighlightType : uint8
{
	NoHighlight,
	PostProcessStencilOutlineOnly,
	TextWidgetOnly,
	TextWidgetAndPPStencilOutline
};

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

	// Will return resulting actor if trace resulted in an actor that responds to the Interactable Interface, otherwise returns nullPtr
	FORCEINLINE TObjectPtr<AActor> GetFocusedActor() {return FocusedActor;}

	// If using EInteractionType::MouseCursor, will return the most current HitResult
	FORCEINLINE FHitResult GetCursorHitResult() {return CursorHit;}
	
protected:
	UFUNCTION(Server, Reliable)
	void ServerInteract(AActor* InFocus);

	void FindBestInteractable();

	void CursorTrace();
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY()
	TObjectPtr<AActor> FocusedActor;

	UPROPERTY()
	TObjectPtr<AActor> PreviousFocusedActor;
	
	UPROPERTY(EditAnywhere, Category="DA|Trace")
	EInteractionType InteractionType;

	UPROPERTY(EditAnywhere, Category="DA|Trace")
	EInteractionHighlightType HighlightType;
	
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

	void HighlightFocusedActor(bool bDebugDraw);
	void ToggleWidgetOnFocusedActor(bool bDebugDraw);
	
	FHitResult CursorHit;
	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
