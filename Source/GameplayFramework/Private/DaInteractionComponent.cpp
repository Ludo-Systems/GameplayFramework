// Fill out your copyright notice in the Description page of Project Settings.


#include "DaInteractionComponent.h"

#include "Blueprint/UserWidget.h"
#include "DaInteractableInterface.h"
#include "GameplayFramework.h"
#include "UI/DaWorldUserWidget.h"

static TAutoConsoleVariable<bool> CVarDebugDrawInteraction(TEXT("da.InteractionDebugDraw"), false, TEXT("Enable Debug Lines For Interact Component."), ECVF_Cheat);

// Sets default values for this component's properties
UDaInteractionComponent::UDaInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	TraceRadius = 30.0f;
	TraceDistance = 500.0f;

	CollisionChannel = ECC_WorldDynamic;
}

void UDaInteractionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	RemoveWidget();
	
	Super::EndPlay(EndPlayReason);
}

void UDaInteractionComponent::RemoveWidget()
{
	bool bDebugDraw = CVarDebugDrawInteraction.GetValueOnGameThread();

	if (DefaultWidgetInstance && DefaultWidgetInstance->IsInViewport())
	{
		DefaultWidgetInstance->RemoveFromParent();
		if (bDebugDraw)
		{
			LogOnScreen(this,"DaInteractionComponent: Removed Widget to Viewport.", true, FColor::Yellow);
		}
	}
}


// Called every frame
void UDaInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	APawn* MyPawn = CastChecked<APawn>(GetOwner());
	if(MyPawn->IsLocallyControlled())
	{
		FindBestInteractable();
	}
}

void UDaInteractionComponent::FindBestInteractable()
{
	bool bDebugDraw = CVarDebugDrawInteraction.GetValueOnGameThread();
	
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);

	AActor* MyOwner = GetOwner();

	FVector EyeLocation;
	FRotator EyeRotation;
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
	
	FVector End = EyeLocation + (EyeRotation.Vector() * TraceDistance);

	FCollisionShape Shape;
	Shape.SetSphere(TraceRadius);
	
	TArray<FHitResult> Hits;
	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape);
	
	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;

	// Clear Focused Actor before hit
	FocusedActor = nullptr;
	
	for (FHitResult Hit : Hits)
	{
		if (bDebugDraw)
		{
			DrawDebugSphere(GetWorld(), Hit.ImpactPoint, TraceRadius, 32, LineColor, false, 1.0f);
		}
		
		if (AActor* HitActor = Hit.GetActor())
		{
			if (HitActor->Implements<UDaInteractableInterface>())
			{
				FocusedActor = HitActor;
				if (bDebugDraw)
				{
					LogOnScreen(this,FString::Printf(TEXT("DaInteractionComponent: FocusedActor: %s."), *GetNameSafe(FocusedActor)), true, FColor::Yellow, 5.f, 1);
				}
				break;
			}
		}
	}

	// focus effect - loads a custom widget onto interactable
	if (FocusedActor)
	{
		// Lazily load the widget when its first needed
		if (DefaultWidgetInstance == nullptr && ensure(DefaultWidgetClass))
		{
			DefaultWidgetInstance = CreateWidget<UDaWorldUserWidget>(GetWorld(), DefaultWidgetClass);
		}

		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->AttachedActor = FocusedActor;

			if (!DefaultWidgetInstance->IsInViewport())
			{
				DefaultWidgetInstance->AddToViewport();
				if (bDebugDraw)
				{
					LogOnScreen(this,"DaInteractionComponent: Added Widget to Viewport.", true, FColor::Yellow);
				}
			}
		}
	}
	else
	{
		RemoveWidget();
	}
	
	if (bDebugDraw)
	{
		DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 1.0f, 0, 2.0f);
	}
}

void UDaInteractionComponent::PrimaryInteract()
{
	if (CVarDebugDrawInteraction.GetValueOnGameThread())
	{
		LogOnScreen(this,FString::Printf(TEXT("DaInteractionComponent::PrimaryInteract FocusedActor: %s."), *GetNameSafe(FocusedActor)), true, FColor::Green, 5.f, 2);
	}

	ServerInteract(FocusedActor);
}

void UDaInteractionComponent::ServerInteract_Implementation(AActor* InFocus)
{
	if (InFocus == nullptr)
	{
		LogOnScreen(this,"DaInteractionComponent: No FocusedActor to Interact.", true, FColor::Red);
		return;
	}

	if (CVarDebugDrawInteraction.GetValueOnGameThread())
	{
		LogOnScreen(this,FString::Printf(TEXT("DaInteractionComponent::ServerInteract_Implementation FocusedActor: %s."), *GetNameSafe(InFocus)), true, FColor::Green, 5.f, 3);
	}
	
	APawn* MyPawn = CastChecked<APawn>(GetOwner());
	IDaInteractableInterface::Execute_Interact(InFocus, MyPawn);
}