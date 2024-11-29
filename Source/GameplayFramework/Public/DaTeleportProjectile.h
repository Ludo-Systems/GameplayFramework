// Copyright Dream Awake Solutions LLC

#pragma once

#include "CoreMinimal.h"
#include "DaProjectile.h"
#include "DaTeleportProjectile.generated.h"

UCLASS()
class GAMEPLAYFRAMEWORK_API ADaTeleportProjectile : public ADaProjectile
{
	GENERATED_BODY()

public:
	ADaTeleportProjectile();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	float TeleportDelay;

	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	float DetonateDelay;
	
	// Handle to cancel timer if we already hit something
	FTimerHandle TimerHandle_DelayedDetonate;

	virtual void Explode_Implementation() override;

	void TeleportInstigator();
	
	virtual void BeginPlay() override;
	
};
