// Fill out your copyright notice in the Description page of Project Settings.


#include "Racetrack/RaceTimers/Sector.h"
#include "Racetrack/Racetrack.h"
#include "Racetrack/RaceTimers/StartFinishLine.h"
#include "RDefinitions.h"
#include "RacepacePlayer.h"
#include "RacecarUIController.h"
#include "RacecarTimingComponent.h"
#include "Racecar.h"

ASector::ASector()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

IMPLEMENT_ENTER_FUNCTION(ASector)
{
	CALL_PARENT_ENTER_FUNCTION();

	if (ARacecar* Racecar = CastToRacecar(OtherActor))
	{
		OnRacecarCross(Racecar);
	}
}

void ASector::OnRacecarCross(ARacecar* Racecar)
{
	ARacepacePlayer* RacepacePlayer = Racecar->GetRacepacePlayerController();
	if (!RacepacePlayer)
	{
		E("ASector::OnRacecarCross: Racecar has no ARacepacePlayer*!");
		return;
	}

	float CurrentLapTime = RacepacePlayer->GetCurrentLapTime();

	if (!CrossOrder.Contains(Racecar))
	{
		CrossOrder.Add(Racecar, CurrentLapTime);
		URacecarTimingComponent* TimingComponent = Racecar->TimingComponent;
		CNULLF(TimingComponent, "%s has no Timing Component", *Racecar->GetName());

		if (StartFinishLine)
		{
			if (URacecarUIController* UI = Racecar->RacecarUIController)
			{
				if (RacepacePlayer->GetLapTimes().Num() == 0)
				{
					UI->SetSplitTime(RacepacePlayer->GetCurrentLapTime());
				}
				else if (RacepacePlayer->HasLapStarted())
				{
					const int32 CurrentSector = RacepacePlayer->CurrentSector;

					float BestDeltaSector, BestLapSector;
					RacepacePlayer->GetBestSplitTimes(CurrentSector, BestDeltaSector, BestLapSector);

					UI->DisplayDeltaSplitTimes(
						TimingComponent->GetLastSectorDeltaTime(), BestDeltaSector,
						RacepacePlayer->GetCurrentLapTime(), BestLapSector
					);
				}
			}
		}

		++RacepacePlayer->CurrentSector;
		TimingComponent->RegisterDeltaTime(RacepacePlayer);
		TimingComponent->RegisterLapCrossTime(RacepacePlayer);
		TimingComponent->StartTimingSector(EnterTime);
	}
	else
	{
		// Only called when the same Racecar enters this Sector multiple times in one lap...

		CrossOrder[Racecar] = RacepacePlayer->GetCurrentLapTime();
	}
}

bool ASector::HasCrossed(const ARacecar* Racecar) const
{
	return CrossOrder.Contains(Racecar);
}

float ASector::GetCrossTime(const ARacecar* Racecar) const
{
	if (CrossOrder.Contains(Racecar))
	{
		return CrossOrder[Racecar];
	}

	return -1.f;
}

void ASector::ClearTimes(const ARacecar* Racecar)
{
	CrossOrder.Remove(Racecar);
}
