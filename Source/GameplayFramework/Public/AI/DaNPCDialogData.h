// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DaNPCDialogData.generated.h"

UENUM(BlueprintType)
enum EDialogEmotionStateEnum
{
	Neutral,
	Frustration,
	Joy,
	Sadness,
	Pain,
	Anger,
	Disgust,
	Fear,
	Happiness,
	Surprise,
	Affection,
	Disappointment,
	Anxiety,
	Pride,
	Shame,
	Defensive,
	Excitement,
	Interest,
	Relaxation,
	EmotionStateMAX
};

UENUM(BlueprintType)
enum EDialogMotivationStateEnum
{
	Unspecified,
	Pleasure,
	Satisfaction,
	Desire,
	DesireToEat,
	DesireToDrink,
	DesireToSleep,
	DesireToPlay,
	DesireToLearn,
	DesireToLove,
	DesireToHate,
	DesireToKill,
	Financial,
	Social,
	Health,
	Mental,
	Physical,
	Emotional,
	MotivationStateMAX
};

UENUM(BlueprintType)
enum EDialogRelationshipStateEnum
{
	Unknown,
	Stranger,
	Acquaintance,
	Colleague,
	Friend,
	Enemy,
	ArchEnemy,
	LoveInterest,
	Love,
	Romance,
	Partner,
	RelationshipStateMAX
};

UENUM(BlueprintType)
enum EDialogNegotiationStateEnum
{
	NotStarted,
	Offer,
	Accept,
	Reject,
	Finalize,
	NoResponse,
	NegotiationStateMAX
};

/**
 * 
 */
UCLASS()
class GAMEPLAYFRAMEWORK_API UDaNPCDialogData : public UObject
{
	GENERATED_BODY()

public:
	UDaNPCDialogData();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialog" )
	TEnumAsByte<EDialogEmotionStateEnum> CurrentEmotionState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialog" )
	TEnumAsByte<EDialogMotivationStateEnum> CurrentMotivationState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialog" )
	TEnumAsByte<EDialogRelationshipStateEnum> CurrentRelationshipState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialog" )
	TEnumAsByte<EDialogNegotiationStateEnum> CurrentNegotiationState;
};
