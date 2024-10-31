// Copyright Dream Awake Solutions LLC


#include "DaPlayerCharacter_ThirdPerson.h"

#include "DaInteractionComponent.h"
#include "CoreGameplayTags.h"
#include "DaPlayerController.h"
#include "GameplayFramework.h"

#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

static TAutoConsoleVariable<bool> CVarDebugPlayerArrows(TEXT("da.DrawPlayerOrientation"), false, TEXT("Enable Debug Draw of Player Actor and Pawn Orientation arrows"), ECVF_Cheat);

// Sets default values
ADaPlayerCharacter_ThirdPerson::ADaPlayerCharacter_ThirdPerson()
{
	CharacterTypeGameplayTag = CoreGameplayTags::TAG_Character_Type_Player_ThirdPerson;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	InteractionComp = CreateDefaultSubobject<UDaInteractionComponent>(TEXT("InteractionComp"));

	// Spring Arm follows pawn rotation
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetUsingAbsoluteRotation(true);

	// set some spring arm defaults for proper targeting and tracing with crosshair in 3rd person
	SpringArmComp->TargetArmLength = 250.0f;
	SpringArmComp->SocketOffset = FVector(0.0f, 90.0f, 0.0f);
	SpringArmComp->SetRelativeLocation(FVector(0.0f, 0.0f, 80.0f));

	// Rotate character towards acceleration direction
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;

	// enable for aim assist or debug draw
	PrimaryActorTick.bCanEverTick = false;
}

FVector ADaPlayerCharacter_ThirdPerson::GetPawnViewLocation() const
{
	return CameraComp->GetComponentLocation();
}

void ADaPlayerCharacter_ThirdPerson::PrimaryInteraction() const
{
	if (InteractionComp) InteractionComp->PrimaryInteract();
}

void ADaPlayerCharacter_ThirdPerson::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FindCrosshairTarget();

	if (CVarDebugPlayerArrows.GetValueOnGameThread())
	{
		// -- Rotation Visualization -- //
		const float DrawScale = 100.0f;
		const float Thickness = 5.0f;

		FVector LineStart = GetActorLocation();
		// Offset to the right of pawn
		LineStart += GetActorRightVector() * 100.0f;
		// Set line end in direction of the actor's forward
		FVector ActorDirection_LineEnd = LineStart + (GetActorForwardVector() * 100.0f);
		// Draw Actor's Direction
		DrawDebugDirectionalArrow(GetWorld(), LineStart, ActorDirection_LineEnd, DrawScale, FColor::Yellow, false, 0.0f, 0, Thickness);

		FVector ControllerDirection_LineEnd = LineStart + (GetControlRotation().Vector() * 100.0f);
		// Draw 'Controller' Rotation ('PlayerController' that 'possessed' this character)
		DrawDebugDirectionalArrow(GetWorld(), LineStart, ControllerDirection_LineEnd, DrawScale, FColor::Green, false, 0.0f, 0, Thickness);
	}
}

void ADaPlayerCharacter_ThirdPerson::FindCrosshairTarget()
{
	// Ignore if not using GamePad
	ADaPlayerController* PC = GetController<ADaPlayerController>();

	// Should use aim assist when currently controlled and using gamepad
	// Note: you *may* always want to line trace if using this result for other things like coloring crosshair or re-using this hit data for aim adjusting during projectile attacks
	if (PC == nullptr)// || !PC->IsUsingGamepad())
	{
		bHasPawnTarget = false;
		return;
	}

	FVector EyeLocation;
	FRotator EyeRotation;
	GetActorEyesViewPoint(EyeLocation, EyeRotation);

	constexpr float AimAssistDistance = 5000.f;
	const FVector TraceEnd = EyeLocation + (EyeRotation.Vector() * AimAssistDistance);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	FCollisionShape Shape;
	Shape.SetSphere(50.f);

	FTraceDelegate Delegate = FTraceDelegate::CreateUObject(this, &ADaPlayerCharacter_ThirdPerson::CrosshairTraceComplete);
	TraceHandle = GetWorld()->AsyncSweepByChannel(EAsyncTraceType::Single, EyeLocation, TraceEnd, FQuat::Identity, ECC_Pawn, Shape, Params, FCollisionResponseParams::DefaultResponseParam, &Delegate);
}

void ADaPlayerCharacter_ThirdPerson::CrosshairTraceComplete(const FTraceHandle& InTraceHandle, FTraceDatum& InTraceDatum)
{
	// at most expect one hit
	if (InTraceDatum.OutHits.IsValidIndex(0))
	{
		FHitResult Hit = InTraceDatum.OutHits[0];
		// Figure out if dealing with a Pawn, may want aim assist on other 'things', which requires a different check
		bHasPawnTarget = Hit.IsValidBlockingHit() && Hit.GetActor()->IsA(APawn::StaticClass());

		LOG("has pawn target: %s", bHasPawnTarget ? TEXT("true") : TEXT("false"));
	}
}

void ADaPlayerCharacter_ThirdPerson::LookStick(const FInputActionValue& InputValue)
{
	FVector2d Value = InputValue.Get<FVector2d>();

	// track if negative
	bool XNegative = Value.X < 0.0f;
	bool YNegative = Value.Y < 0.0f;
	
	// nonlinear makes aim easier
	Value = Value*Value;

	XNegative?Value.X*=-1.f:Value.X*=1.f;
	YNegative?Value.Y*=-1.f:Value.Y*=1.f;

	// @TODO: futher modify with sensativity settings
	static const float LookYawRate = 100.0f;
	static const float LookPitchRate = 50.0f;

	// Aim assist
	// todo: may need to ease this out and/or change strength based on distance to target
	float RateMultiplier = 1.0f;
	if (bHasPawnTarget)
	{
		RateMultiplier = 0.5f;
	}
	
	// @todo: increase accelerate longer input is > 0 on axis
	const float deltaSeconds = GetWorld()->GetDeltaSeconds();
	AddControllerYawInput(Value.X*(LookYawRate*RateMultiplier)*deltaSeconds);
	AddControllerPitchInput(Value.Y*(LookPitchRate*RateMultiplier)*deltaSeconds);
}