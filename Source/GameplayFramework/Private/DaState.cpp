// Copyright Dream Awake Solutions LLC


#include "DaState.h"

UDaState* UDaBranch::TryBranch(const UObject* RefObject, const TArray<UDaInputAtom*>& DataSource, int32 DataIndex,
	int32& OutDataIndex)
{
	OutDataIndex = DataIndex+1;
	if (DataSource.IsValidIndex(DataIndex) && AcceptableInputs.Contains(DataSource[DataIndex]))
	{
		return bReverseInputTest ? nullptr : DestinationState;
	}
	return bReverseInputTest ? DestinationState : nullptr;
}

UDaState::UDaState()
{
	bLoopByDefault = true;
}

FStateMachineResult UDaState::RunState(const UObject* RefObject, const TArray<UDaInputAtom*>& DataSource,
	int32 DataIndex, int32 RemainingSteps)
{
	bool bMustEndNow = (bTerminateImmediatly || !DataSource.IsValidIndex(DataIndex));

	// if we are still running, see where our branches lead
	if (RemainingSteps && !bMustEndNow)
	{
		UDaState* DestinationState = nullptr;
		int32 DestinationDataIndex = DataIndex;
		for (int32 i = 0; i < InstancedBranches.Num(); i++)
		{
			if (ensure(InstancedBranches[i]))
			{
				DestinationState = InstancedBranches[i]->TryBranch(RefObject, DataSource, DataIndex, DestinationDataIndex);
				if (DestinationState)
				{
					return DestinationState->RunState(RefObject, DataSource, DestinationDataIndex, RemainingSteps-1);
				}
			}
		}

		for (int32 i = 0; i < SharedBranches.Num(); i++)
		{
			if (ensure(SharedBranches[i]))
			{
				DestinationState = SharedBranches[i]->TryBranch(RefObject, DataSource, DataIndex, DestinationDataIndex);
				if (DestinationState)
				{
					return DestinationState->RunState(RefObject, DataSource, DestinationDataIndex, RemainingSteps-1);
				}
			}
		}
		
		// We didnt find a branch, so end state unless we're told to loop
		if (bLoopByDefault)
		{
			return LoopState(RefObject, DataSource, DataIndex, RemainingSteps);
		}
		bMustEndNow = true;
	}

	FStateMachineResult SMR;
	SMR.FinalState = this;
	SMR.DataIndex = DataIndex;
	SMR.CompletionType = bMustEndNow ? CompletionType : EStateMachineCompletionType::OutOfSteps;
	return SMR;
}

FStateMachineResult UDaState::LoopState(const UObject* RefObject, const TArray<UDaInputAtom*>& DataSource,
	int32 DataIndex, int32 RemainingSteps)
{
	// By Default, increase data index by 1 and decrease remaining steps by 1.
	return RunState(RefObject, DataSource, DataIndex + 1, RemainingSteps - 1);
}

