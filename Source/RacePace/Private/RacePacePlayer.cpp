// Fill out your copyright notice in the Description page of Project Settings.


#include "RacePacePlayer.h"
#include "RacePaceHelpers.h"

#include "Racecar.h"

#include "RacecarUIController.h"


ARacePacePlayer::ARacePacePlayer()
{

}

void ARacePacePlayer::BeginPlay()
{
	Super::BeginPlay();

	// ...

}

void ARacePacePlayer::AddLapTime(const float& NewLapTime)
{
	LapTimes.Add(NewLapTime);
}

TArray<float> ARacePacePlayer::GetLapTimes() const
{
	return LapTimes;
}

float ARacePacePlayer::GetLastLapTime() const
{
	if (LapTimes.Num() > 0)
	{
		return LapTimes[LapTimes.Num() - 1];
	}

	return 0.f;
}


void ARacePacePlayer::ClearLapTimes()
{
	LapTimes.Empty();
}

