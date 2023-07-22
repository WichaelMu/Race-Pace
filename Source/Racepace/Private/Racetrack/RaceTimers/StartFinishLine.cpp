// Fill out your copyright notice in the Description page of Project Settings.


#include "Racetrack/RaceTimers/StartFinishLine.h"
#include "Racetrack/RaceTimers/Sector.h"
#include "Racetrack/Racetrack.h"
#include "RacepacePlayer.h"
#include "Racecar.h"
#include "RacecarUIController.h"
#include "RDefinitions.h"

AStartFinishLine::AStartFinishLine()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bAllowContinuousLaps = true;
}

IMPLEMENT_ENTER_FUNCTION(AStartFinishLine)
{
	CALL_PARENT_ENTER_FUNCTION();

	if (ARacecar* Racecar = CastToRacecar(OtherActor))
	{
		OnRacecarCross(Racecar);
	}
}


void AStartFinishLine::OnRacecarCross(ARacecar* Racecar)
{
	ARacepacePlayer* Player = Racecar->GetRacepacePlayerController();
	CNULLF(Player, "%s has no valid ARacepacePlayer", *Racecar->GetName());

	bool bHasPreviouslyCrossed = Grid.Contains(Racecar);
	bool bWasLapValid = WasLapValid(Racecar);

	if (bAllowContinuousLaps)
	{

		if (bHasPreviouslyCrossed && bWasLapValid)
		{
			Player->StopLap(true);
			SetLapUI(Racecar, Player, GetLapTime());
		}
		else
		{
			Grid.Add(Racecar, ERacetrackLapType::Hotlap);
		}

		if (bWasLapValid || !Player->HasLapStarted())
		{
			Player->StartLap(EnterTime);
			StartTime = EnterTime;
		}
	}
	else
	{
		// This Racecar has just ended a lap.
		if (bHasPreviouslyCrossed)
		{
			if (bWasLapValid)
			{
				Player->StopLap(false);
				SetLapUI(Racecar, Player, GetLapTime());

				Grid.Remove(Racecar);
			}
		}
		else
		{
			Player->StartLap(EnterTime);
			StartTime = EnterTime;

			Grid.Add(Racecar, ERacetrackLapType::Hotlap);
		}
	}

	// Times are cleared regardless of their validity.
	ClearSectors(Racecar);
}

float AStartFinishLine::GetLapTime() const
{
	double TimeSince = GetWorld()->TimeSince(StartTime);
	return (float)TimeSince;
}

void AStartFinishLine::SetLapUI(ARacecar* Racecar, ARacepacePlayer* Player, float LapTime)
{
	if (URacecarUIController* UI = Racecar->RacecarUIController)
	{
		UI->SetLastLapTime(LapTime);

		float BestTime = Player->GetBestLapTime();
		Player->AddLapTime(LapTime);

		if (BestTime != 0.f)
		{
			UI->CompareLapToBestDeltas(LapTime, BestTime);
			UI->SetBestLapTime(Player->GetBestLapTime());
		}
	}
}

bool AStartFinishLine::WasLapValid(const ARacecar* Racecar) const
{
	ELapCompletionResult Result = ELapCompletionResult::SectorsNotCrossed;

	if (Sectors.IsEmpty())
	{
		Result = ELapCompletionResult::ValidLap;
		return true;
	}

	bool bWasCrossedSequentially = true;

	for (int32 i = 0; i < Sectors.Num() - 1; ++i)
	{
		ASector* Sector = Sectors[i];
		ASector* NextSector = Sectors[i + 1];

		// Ensure every Sector was crossed.
		if (!Sector->HasCrossed(Racecar))
		{
			LF("%s not crossed", *Sector->GetName());
			Result = ELapCompletionResult::SectorsNotCrossed;

			return false;
		}

		// Ensure Sectors were crossed in sequential order.
		if (Sector->GetCrossTime(Racecar) > NextSector->GetCrossTime(Racecar))
		{
			LF("%s: %f and %s: %f not sequential", *Sector->GetName(), Sector->GetCrossTime(Racecar), *NextSector->GetName(), NextSector->GetCrossTime(Racecar));
			Result = ELapCompletionResult::SectorsNotSequential;

			return false;
		}
	}

	// Also ensure the last Sector has also been crossed.
	return Sectors.Last()->HasCrossed(Racecar);
}

void AStartFinishLine::ClearSectors(const ARacecar* Racecar)
{
	for (int32 i = 0; i < Sectors.Num(); ++i)
	{
		Sectors[i]->ClearTimes(Racecar);
	}
}