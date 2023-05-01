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

void ARacepacePlayer::AddLapTime(const float& NewLapTime)
{
}

TArray<float> ARacepacePlayer::GetLapTimes() const
{
	return TArray<float>();
}

float ARacepacePlayer::GetLastLapTime() const
{
	return 0.0f;
}

void ARacepacePlayer::ClearLapTimes()
{
}
