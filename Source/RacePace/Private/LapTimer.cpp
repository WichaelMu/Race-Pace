// Fill out your copyright notice in the Description page of Project Settings.


#include "LapTimer.h"
#include "RacePaceHelpers.h"
#include "RacePacePlayer.h"

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


void ULapTimer::BeginPlay()
{
	Super::BeginPlay();

	// ...

	if (APawn* Racecar = Cast<APawn>(GetOwner()))
	{
		if (ARacePacePlayer* Player = Cast<ARacePacePlayer>(Racecar->GetController()))
		{
			PlayerController = Player;
		}
	}
}


// Called every frame
void ULapTimer::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	if (bIsCurrentlyOnHotlap)
	{
#if USE_DELTATIMING
		ElapsedLapTime += DeltaTime;
#else
		ElapsedLapTime = GetCurrentLapTime();
#endif

		if (PlayerController)
		{
			const FString LapTime = GetTime(ElapsedLapTime);
			PlayerController->SetLapTime(LapTime);
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
	LapTimes.Add(ElapsedLapTime);

	if (PlayerController)
	{
		PlayerController->CompareLapToBestDeltas(ElapsedLapTime, BestLapTime, this);
		PlayerController->SetLastLapTime(GetTime(ElapsedLapTime));

		if (ElapsedLapTime < BestLapTime)
		{
			BestLapTime = ElapsedLapTime;
			const FString BestTime = GetTime(BestLapTime);
			PlayerController->SetBestLapTime(BestTime);
		}
	}

#if USE_DELTATIMING
	ElapsedLapTime = 0.f;
#endif
}


void ULapTimer::ParseTime(const float& RawTime, int32& Minutes, int32& Seconds, int32& Milliseconds)
{
	Seconds = (int)RawTime;

	Milliseconds = (RawTime - Seconds) * 1000;

	Minutes = (int)(Seconds / 60);

	Seconds %= 60;
}

float ULapTimer::GetCurrentLapTime() const
{
#if !USE_DELTATIMING
	return (float)(FPlatformTime::Seconds() - StartRawTime);
#endif

	return ElapsedLapTime;
}

FString ULapTimer::GetTime(const float& InTime) const
{
	int32 Minutes, Seconds, Milliseconds;
	ParseTime(InTime, Minutes, Seconds, Milliseconds);

	return FString::Printf(TEXT("%i:%02i.%.3i"), Minutes, Seconds, Milliseconds);
}


float ULapTimer::GetLastLapTime() const
{
	if (LapTimes.Num() > 0)
	{
		return LapTimes[LapTimes.Num() - 1];
	}

	return 0.f;
}


void ULapTimer::ClearLapTimes()
{
	LapTimes.Empty();
}
