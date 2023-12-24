// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RacepacePlayer.generated.h"

class ARacecar;
class ASector;

/**
 *
 */
UCLASS()
class RACEPACE_API ARacepacePlayer : public APlayerController
{
	GENERATED_BODY()

public:

	ARacepacePlayer();


protected:

	virtual void BeginPlay() override;


public:

	void AddLapTime(const float NewLapTime);
	FORCEINLINE TArray<float> GetLapTimes() const;
	float GetAccumulatedLapTimes() const;
	float GetBestLapTime() const;
	float GetLastLapTime() const;
	void ClearLapTimes();

	FORCEINLINE bool HasLapStarted(float& StartTime) const;
	FORCEINLINE bool HasLapStarted() const;
	FORCEINLINE void StartLap(const float StartTime);
	FORCEINLINE void StopLap(bool bContinuousLap);

	FORCEINLINE ARacecar* GetRacecar() const;

	FORCEINLINE bool IsBestLapTime(const float InLapTime) const;
	void SetBestTimes();
	FORCEINLINE void GetBestSplitTimes(const int32 Sector, float& BestDeltaSector, float& BestLapCrossSector);

	float GetCurrentLapTime() const;

public:

	int32 CurrentSector;

private:

	UPROPERTY(VisibleAnywhere, Category = "Lap Records")
		TArray<float> LapTimes;

	// Best times between Sectors.
	TArray<float> BestDeltaSectors;
	// Best lap interval between Sectors.
	TArray<float> BestLapSectors;

	float LapStartTime;
	bool bLapHasStarted;

};
