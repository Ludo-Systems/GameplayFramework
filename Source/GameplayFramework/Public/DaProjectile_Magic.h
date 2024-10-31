// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DaProjectile.h"
#include "DaProjectile_Magic.generated.h"

class UGameplayEffect;
/**
 * 
 */
UCLASS()
class GAMEPLAYFRAMEWORK_API ADaProjectile_Magic : public ADaProjectile
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, Category="Damage")
	float DamageAmount;

	UPROPERTY(EditAnywhere, Category="Damage")
	TSubclassOf<UGameplayEffect> DamageEffect;
	
	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
public:

	ADaProjectile_Magic();
};
