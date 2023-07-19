// Fill out your copyright notice in the Description page of Project Settings.


#include "Racetrack/RaceTimers/StartFinishLine.h"
#include "Racetrack/Racetrack.h"
#include "RacepacePlayer.h"
#include "Racecar.h"
#include "RacecarUIController.h"
#include "RDefinitions.h"

AStartFinishLine::AStartFinishLine()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

IMPLEMENT_ENTER_FUNCTION(AStartFinishLine)
{
	Super::Enter(OverlappedComponent, OtherActor, OtherComponent, OtherBodyIndex, bFromSweep, SweepResult);

	if (ARacecar* Racecar = CastToRacecar(OtherActor))
	{
		OnRacecarCross(Racecar);
	}
}


void AStartFinishLine::OnRacecarCross(ARacecar* Racecar)
{
	ARacepacePlayer* Player = Racecar->GetRacepacePlayerController();
	CNULLF(Player, "%s has no valid ARacepacePlayer", *Racecar->GetName())

	// This Racecar has just ended a lap.
	if (Grid.Contains(Racecar))
	{
		float LapTime = GetLapTime();

		if (URacecarUIController* UI = Racecar->RacecarUIController)
		{
			UI->SetLastLapTime(LapTime);

			float BestTime = Player->GetBestLapTime();
			Player->AddLapTime(LapTime);

			if (BestTime != 0.f)
			{
				UI->CompareLapToBestDeltas(LapTime, BestTime);
				UI->SetBestLapTime(BestTime);
			}
		}

		Player->StopLap();
		Grid.Remove(Racecar);
	}
	else
	{
		StartTime = EnterTime;

		Player->StartLap(StartTime);

		Grid.Add(Racecar, ERacetrackLapType::Hotlap);
	}
}

float AStartFinishLine::GetLapTime() const
{
	double TimeSince = GetWorld()->TimeSince(StartTime);
	return (float)TimeSince;
}
