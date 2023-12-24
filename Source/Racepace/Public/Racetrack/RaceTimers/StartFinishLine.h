// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Racetrack/RaceTimers/TrackTimerBase.h"
#include "Containers/Map.h"
#include "Racetrack/Racetrack.h"
#include "StartFinishLine.generated.h"

class ARacepacePlayer;
class ARacecar;

/**
 *
 */
UCLASS()
class RACEPACE_API AStartFinishLine : public ATrackTimerBase
{
	GENERATED_BODY()


public:
	// Sets default values for this actor's properties
	AStartFinishLine();

	virtual void BeginPlay() override;


public:

	OVERRIDE_ENTER_FUNCTION()

	void OnRacecarCross(ARacecar* Racecar);
	FORCEINLINE float GetLapStartTime() const;
	FORCEINLINE float GetSectorTime(const ARacecar* Racecar, const int32 Sector) const;


private:

	float GetLapTime() const;
	void SetLapUI(ARacecar* Racecar, ARacepacePlayer* Player, float LapTime);

	bool WasLapValid(const ARacecar* Racecar) const;
	void OnLapEnd(ARacecar* Racecar);

private:

	UPROPERTY(EditInstanceOnly)
		TArray<ASector*> Sectors;

	TMap<ARacecar*, ERacetrackLapType> Grid;
	float StartTime;

	UPROPERTY(EditAnywhere)
		bool bAllowContinuousLaps;

};
