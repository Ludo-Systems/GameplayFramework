// Fill out your copyright notice in the Description page of Project Settings.


#include "DaTeleportProjectile.h"

#include "NiagaraComponent.h"
#include "Components/AudioComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"


ADaTeleportProjectile::ADaTeleportProjectile()
{
	TeleportDelay = 0.2f;
	DetonateDelay = 0.2f;
	
	MovementComp->InitialSpeed = 6000.f;
}

void ADaTeleportProjectile::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_DelayedDetonate, this, &ADaTeleportProjectile::Explode, DetonateDelay);
}

void ADaTeleportProjectile::Explode_Implementation()
{
	// Clear timer if the Explode was already called through another source like OnActorHit
	GetWorldTimerManager().ClearTimer(TimerHandle_DelayedDetonate);
	
	UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
	UGameplayStatics::SpawnEmitterAtLocation(this, HitEffect, GetActorLocation(), GetActorRotation());

	AudioComp->Stop();
	EffectComp->Deactivate();

	MovementComp->StopMovementImmediately();
	SetActorEnableCollision(false);

	FTimerHandle TimerHandle_DelayedTeleport;
	GetWorldTimerManager().SetTimer(TimerHandle_DelayedTeleport, this, &ADaTeleportProjectile::TeleportInstigator, TeleportDelay);

	// need to override super since we handle destroy in the timer callback
	//Super::Explode_Implementation();
}

void ADaTeleportProjectile::TeleportInstigator()
{
	AActor* ActorToTeleport = GetInstigator();
	// Must be set in spawning code (also good for things like knowing who caused the damage)
	check(ActorToTeleport);
	
	// Keep instigator rotation or it may end up jarring
	ActorToTeleport->TeleportTo(GetActorLocation(), ActorToTeleport->GetActorRotation(), false, false);

	// Play shake on the player we teleported
	const APawn* InstigatorPawn = CastChecked<APawn>(ActorToTeleport);
	APlayerController* PC = InstigatorPawn->GetController<APlayerController>();
	// Controller can be nullptr if we died (and detached the pawn) just after launching the dash projectile
	if (PC && PC->IsLocalController())
	{
		PC->ClientStartCameraShake(ImpactShake);
	}

	// Now we're ready to destroy self
	Destroy();
}


