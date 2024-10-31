// Copyright Dream Awake Solutions LLC

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DaState.generated.h"

class UDaState;

UENUM()
enum class EStateMachineCompletionType : uint8
{
	// Implicit failure 
	NotAccepted,

	// success
	Accepted,

	// explicit failure
	Rejected,

	// simulation ran out of steps while still running
	OutOfSteps UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct GAMEPLAYFRAMEWORK_API FStateMachineResult
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	EStateMachineCompletionType CompletionType;

	UPROPERTY()
	UDaState* FinalState;

	UPROPERTY()
	int32 DataIndex;

	FStateMachineResult() : CompletionType(EStateMachineCompletionType::NotAccepted), FinalState(nullptr), DataIndex(-1) {}
};

/**
 * 
 */
UCLASS()
class GAMEPLAYFRAMEWORK_API UDaInputAtom : public UDataAsset
{
	GENERATED_BODY()

public:

	// display just for debugging purposes
	UPROPERTY(EditAnywhere)
	FName Description;
};

UCLASS(EditInlineNew)
class GAMEPLAYFRAMEWORK_API UDaBranch : public UDataAsset
{
	GENERATED_BODY()

public:
	// Returns DestinationState on Success, NULL on failure, For subclasses OutDataIndex might be something other than 1, if a branch is made to consume mutliple sources.
	virtual UDaState* TryBranch(const UObject* RefObject, const TArray<UDaInputAtom*>& DataSource, int32 DataIndex, int32 &OutDataIndex);
	
protected:
	// State where we will go next if this branch is taken
	UPROPERTY(EditAnywhere)
	UDaState* DestinationState;

	// If true, the meaning of acceptable inputs is reversed
	UPROPERTY(EditAnywhere)
	uint32 bReverseInputTest : 1;

	UPROPERTY(EditAnywhere)
	TArray<UDaInputAtom *> AcceptableInputs;
};

/**
 * 
 */
UCLASS()
class GAMEPLAYFRAMEWORK_API UDaState : public UDataAsset
{
	GENERATED_BODY()

public:

	UDaState();

	/* Attempt to run the state with the input from the given DataSource object
	 * Will start reading input at data index
	 * Will decrement RemainingSteps and automatically fail after it hits 0
	 */
	UFUNCTION(BlueprintCallable, Category="State Machine")
	virtual FStateMachineResult RunState(const UObject* RefObject, const TArray<UDaInputAtom*>& DataSource, int32 DataIndex = 0, int32 RemainingSteps = -1);

protected:

	// Loop. Used when input atom being processed isn't recognized
	virtual FStateMachineResult LoopState(const UObject* RefObject, const TArray<UDaInputAtom*>& DataSource, int32 DataIndex, int32 RemainingSteps);

	// If input runs out on this state (or TerminateImmediatly is true), this is how the result will be interpreted
	UPROPERTY(EditAnywhere)
	EStateMachineCompletionType CompletionType;

	// A state machine run that enters this state will terminate immediatly
	UPROPERTY(EditAnywhere)
	uint32 bTerminateImmediatly : 1;

	// If this is set this state will loop on itself whenever unhandled input is detected
	UPROPERTY(EditAnywhere)
	uint32 bLoopByDefault : 1;

	// Branches to other states. DFA. These are in priority order, so the first succesful branch will be taken
	UPROPERTY(EditAnywhere, Instanced)
	TArray<UDaBranch*> InstancedBranches;

	// Branches to other states. DFA. These are in priority order, so the first succesful branch will be taken. These will run after InstancedBranches
	UPROPERTY(EditAnywhere)
	TArray<UDaBranch*> SharedBranches;
};
