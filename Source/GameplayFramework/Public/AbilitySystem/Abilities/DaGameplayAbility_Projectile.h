// Copyright Dream Awake Solutions LLC

#pragma once

#include "CoreMinimal.h"
#include "DaGameplayAbilityBase.h"
#include "DaGameplayAbility_Projectile.generated.h"

UENUM()
enum class ETargetLocationHitType : uint8
{
	NoHit,
	SphereTrace,
	MouseCursor
};

/**
 * 
 */
UCLASS()
class GAMEPLAYFRAMEWORK_API UDaGameplayAbility_Projectile : public UDaGameplayAbilityBase
{
	GENERATED_BODY()

public:
	UDaGameplayAbility_Projectile(const FObjectInitializer& ObjectInitializer);

	// Use TargetDataUnderCursor for MouseCursor type, or GetTargetLocation_SphereTrace for SphereTrace type
	UFUNCTION(BlueprintCallable)
	void SpawnProjectile(const FVector& ProjectileTargetLocation);

	// Convenience function. Call instead of SpawnProjectile, will play a montage and wait for AnimNotify then
	// spawn projectile at character socket. This only works or Sphere trace currently. 
	UFUNCTION(BlueprintCallable)
	void PlayMontageAndSpawnProjectile();

	UFUNCTION(BlueprintCallable)
	FVector GetTargetLocation_SphereTrace();

protected:

	UPROPERTY(EditAnywhere, Category="Targeting")
	ETargetLocationHitType HitType;
	
	/* If Sphere tracing for target location, Sphere radius of the sweep to find desired target under crosshair. Adjusts final projectile direction */
	UPROPERTY(EditAnywhere, Category="Targeting")
	float SweepRadius;

	/* If Sphere tracing for target location, Fallback distance when sweep finds no collision under crosshair. Adjusts final projectile direction */
	UPROPERTY(EditAnywhere, Category="Targeting")
	float SweepDistanceFallback;

	/* Adjusts final projectile direction, Set to 0 to set Pitch to zero and force particles to move along the plane, otherwise will be calculated in 3D space */
	UPROPERTY(EditAnywhere, Category="Targeting")
	float PitchAdjustmentMultiplier = 1.0f;
	
	/* Time to wait for projectile to hit something before calling end ability and allowing another UDaGameplayAbility_Projectile to activate */
	UPROPERTY(EditAnywhere, Category="Attack")
	float WaitHitDelay;
	
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass;

	/* Particle System played during attack animation */
	UPROPERTY(EditAnywhere, Category = "Attack")
	TObjectPtr<UParticleSystem> CastingEffect;

	/* Sound Effect to play (Can be Wave or Cue) */
	UPROPERTY(EditAnywhere, Category = "Attack")
	TObjectPtr<USoundBase> CastingSound;

	void CalculateTargetLocationAndSpawnProjectile();

	UFUNCTION()
	void OnCursorTargetLocationReceived(const FGameplayAbilityTargetDataHandle& DataHandle);
	
	UFUNCTION()
	void AnimNotifyEventCallback(FGameplayEventData Payload);
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:
	FTimerHandle TimerHandle_EndDelay;

};
