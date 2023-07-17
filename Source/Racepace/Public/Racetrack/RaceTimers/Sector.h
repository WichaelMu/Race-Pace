// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Racetrack/RaceTimers/TrackTimerBase.h"
#include "Sector.generated.h"

/**
 * 
 */
UCLASS()
class RACEPACE_API ASector : public ATrackTimerBase
{
	GENERATED_BODY()

public:

	ASector();


private:

	UPROPERTY(VisibleAnywhere)
		bool bHasBeenCrossed;
	
};
