// Fill out your copyright notice in the Description page of Project Settings.


#include "Racetrack/RaceTimers/Sector.h"
#include "Racetrack/Racetrack.h"
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
	if (!CrossOrder.Contains(Racecar))
	{
		CrossOrder.Add(Racecar, EnterTime);
	}
	else
	{
		CrossOrder[Racecar] = EnterTime;
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
