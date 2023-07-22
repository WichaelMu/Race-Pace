// Fill out your copyright notice in the Description page of Project Settings.


#include "RacepacePlayer.h"
#include "Racecar.h"

#include "RDefinitions.h"

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


bool ARacepacePlayer::HasLapStarted(float& StartTime) const
{
	StartTime = LapStartTime;
	return bLapHasStarted;
}

bool ARacepacePlayer::HasLapStarted() const
{
	return bLapHasStarted;
}

void ARacepacePlayer::StartLap(const float StartTime)
{
	LapStartTime = StartTime;
	bLapHasStarted = true;
}

void ARacepacePlayer::StopLap(bool bContinuousLap)
{
	LapStartTime = 0.f;
	bLapHasStarted = bContinuousLap;
}

ARacecar* ARacepacePlayer::GetRacecar() const
{
	if (ARacecar* Racecar = Cast<ARacecar>(GetPawn()))
	{
		return Racecar;
	}

	E("Controlled Pawn is not a Racecar!");
	return nullptr;
}

