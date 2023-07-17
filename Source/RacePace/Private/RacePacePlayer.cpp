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
