// Fill out your copyright notice in the Description page of Project Settings.


#include "RacepacePlayer.h"
#include "Racecar.h"
#include "Racetrack/RaceTimers/Sector.h"
#include "RacecarTimingComponent.h"

#include "RDefinitions.h"

ARacepacePlayer::ARacepacePlayer()
{
	CurrentSector = 0;
	LapStartTime = 0.f;
	bLapHasStarted = false;
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

float ARacepacePlayer::GetAccumulatedLapTimes() const
{
	float RetVal = 0.f;
	for (float F : GetLapTimes())
	{
		RetVal += F;
	}

	return RetVal;
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

bool ARacepacePlayer::IsBestLapTime(const float InLapTime) const
{
	const float CurrentBestLapTime = GetBestLapTime();
	return InLapTime <= CurrentBestLapTime;
}

void ARacepacePlayer::SetBestTimes()
{
	if (ARacecar* Racecar = GetRacecar())
	{
		if (URacecarTimingComponent* TimingComponent = Racecar->TimingComponent)
		{
			BestLapSectors = TimingComponent->GetFloatValues(TimingComponent->LapCrossTimes);
			BestDeltaSectors = TimingComponent->GetFloatValues(TimingComponent->DeltaTimes);
		}
	}
}

void ARacepacePlayer::GetBestSplitTimes(const int32 Sector, float& BestDeltaSector, float& BestLapCrossSector)
{
	if (!BestLapSectors.IsValidIndex(Sector))
	{
		EF("ARacepacePlayer::GetBestSplitTimes: Sector exceeded BestLapSectors. Sector: %i Num: BestLapSectors.Num(): %i", Sector, BestLapSectors.Num());
		BestDeltaSector = -1.f;
		BestLapCrossSector = -1.f;
		return;
	}

	if (!BestDeltaSectors.IsValidIndex(Sector))
	{
		EF("ARacepacePlayer::GetBestSplitTimes: Sector exceeded BestDeltaSectors. Sector: %i Num: BestDeltaSectors.Num(): %i", Sector, BestDeltaSectors.Num());
		BestDeltaSector = -1.f;
		BestLapCrossSector = -1.f;
		return;
	}

	BestDeltaSector = BestDeltaSectors[Sector];
	BestLapCrossSector = BestLapSectors[Sector];
}

float ARacepacePlayer::GetCurrentLapTime() const
{
	if (HasLapStarted())
	{
		return (float)GetWorld()->TimeSince(LapStartTime);
	}

	return 0.f;
}

