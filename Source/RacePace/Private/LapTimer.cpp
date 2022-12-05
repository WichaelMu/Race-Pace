// Fill out your copyright notice in the Description page of Project Settings.


#include "LapTimer.h"
#include "Racecar.h"
#include "RacePaceHelpers.h"
#include "RacePacePlayer.h"
#include "RacecarUIController.h"

// Sets default values for this component's properties
ULapTimer::ULapTimer()
{
	PrimaryComponentTick.bCanEverTick = true;
#if USE_DELTATIMING
	bIsCurrentlyOnHotlap = false;
	ElapsedLapTime = 0.f;
#else
	StartRawTime = 0.0;
#endif

	BestLapTime = INT_MAX;
}


// Called every frame
void ULapTimer::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	if (bIsCurrentlyOnHotlap)
	{
		ElapsedLapTime += DeltaTime;

		if (RacecarUIController)
		{
			const FString LapTime = GetTime(ElapsedLapTime);
			RacecarUIController->SetLapTime(LapTime);
		}
	}
	else
	{
		PrimaryComponentTick.bCanEverTick = false;
	}
}

void ULapTimer::BeginLap()
{
	ElapsedLapTime = 0.f;
	bIsCurrentlyOnHotlap = true;
	PrimaryComponentTick.bCanEverTick = true;
#if !USE_DELTATIMING
	StartRawTime = FPlatformTime::Seconds();
#endif
}


void ULapTimer::EndLap()
{
	bIsCurrentlyOnHotlap = false;
	ElapsedLapTime = GetCurrentLapTime();

	if (RacecarUIController)
	{
		if (ARacePacePlayer* Player = Racecar->GetRacepacePlayerController())
		{
			Player->AddLapTime(ElapsedLapTime);

			if (Player->GetLapTimes().Num() > 1)
			{
				RacecarUIController->CompareLapToBestDeltas(ElapsedLapTime, BestLapTime);
			}
		}

		RacecarUIController->SetLastLapTime(GetTime(ElapsedLapTime));

		if (ElapsedLapTime < BestLapTime)
		{
			BestLapTime = ElapsedLapTime;
			const FString BestTime = GetTime(BestLapTime);
			RacecarUIController->SetBestLapTime(BestTime);
		}
	}

#if USE_DELTATIMING
	ElapsedLapTime = 0.f;
#endif
}


float ULapTimer::GetCurrentLapTime() const
{
#if !USE_DELTATIMING
	return (float)(FPlatformTime::Seconds() - StartRawTime);
#endif

	return ElapsedLapTime;
}


void ULapTimer::ParseTime(const float& RawTime, int32& Minutes, int32& Seconds, int32& Milliseconds)
{
	Seconds = (int)RawTime;

	Milliseconds = (RawTime - Seconds) * 1000;

	Minutes = (int)(Seconds / 60);

	Seconds %= 60;
}


FString ULapTimer::GetTime(const float& InTime)
{
	int32 Minutes, Seconds, Milliseconds;
	ParseTime(InTime, Minutes, Seconds, Milliseconds);

	return FString::Printf(TEXT("%i:%02i.%.3i"), Minutes, Seconds, Milliseconds);
}