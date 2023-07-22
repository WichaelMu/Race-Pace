// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RacepacePlayer.generated.h"

class ARacecar;

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
	float GetBestLapTime() const;
	float GetLastLapTime() const;
	void ClearLapTimes();

	FORCEINLINE bool HasLapStarted(float& StartTime) const;
	FORCEINLINE bool HasLapStarted() const;
	FORCEINLINE void StartLap(const float StartTime);
	FORCEINLINE void StopLap(bool bContinuousLap);

	FORCEINLINE ARacecar* GetRacecar() const;

private:

	UPROPERTY(VisibleAnywhere, Category = "Lap Records")
		TArray<float> LapTimes;

	float LapStartTime;
	bool bLapHasStarted;

};
