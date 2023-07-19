// Fill out your copyright notice in the Description page of Project Settings.


#include "RacepacePlayer.h"

ARacepacePlayer::ARacepacePlayer()
{
}

void ARacepacePlayer::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

void ARacepacePlayer::AddLapTime(const float NewLapTime)
{
	LapTimes.Add(NewLapTime);
}

TArray<float> ARacepacePlayer::GetLapTimes() const
{
	return LapTimes;
}

float ARacepacePlayer::GetBestLapTime() const
{
	if (LapTimes.Num() == 0)
	{
		return 0.f;
	}

	return FMath::Min(LapTimes);
}

float ARacepacePlayer::GetLastLapTime() const
{
	return LapTimes.Last();
}

void ARacepacePlayer::ClearLapTimes()
{
	LapTimes.Empty();
}


bool ARacepacePlayer::HasLapStarted(float& LapTime) const
{
	LapTime = LapStartTime;
	return bLapHasStarted;
}

void ARacepacePlayer::StartLap(const float StartTime)
{
	LapStartTime = StartTime; bLapHasStarted = true;
}

void ARacepacePlayer::StopLap()
{
	LapStartTime = 0.f;
	bLapHasStarted = false;
}

