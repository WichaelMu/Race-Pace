// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Racetrack/RaceTimers/TrackTimerBase.h"
#include "Sector.generated.h"

class ARacecar;
class AStartFinishLine;

/**
 * 
 */
UCLASS()
class RACEPACE_API ASector : public ATrackTimerBase
{
	GENERATED_BODY()

public:

	ASector();

public:

	OVERRIDE_ENTER_FUNCTION();

	FORCEINLINE bool HasCrossed(const ARacecar* Racecar) const;
	FORCEINLINE float GetCrossTime(const ARacecar* Racecar) const;
	FORCEINLINE void ClearTimes(const ARacecar* Racecar);

private:

	void OnRacecarCross(ARacecar* Racecar);

public:

	AStartFinishLine* StartFinishLine;

private:

	// The real World-time of the time a Racecar crossed this Sector.
	TMap<ARacecar*, float> CrossOrder;
	
	UPROPERTY(VisibleAnywhere)
		bool bHasBeenCrossed;
};
