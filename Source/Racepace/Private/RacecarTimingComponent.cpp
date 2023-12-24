// Racepace is a game built by WichaelMu.


#include "RacecarTimingComponent.h"
#include "RDefinitions.h"
#include "Racecar.h"
#include "RacepacePlayer.h"
#include "Engine/World.h"

// Sets default values for this component's properties
URacecarTimingComponent::URacecarTimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...

	InterSectorTiming = 0.f;
}


// Called when the game starts
void URacecarTimingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	// Preliminary Checks.
	CNULLF(GetOwner(), "No Owner!");
	CNULLF(Racecar, "No Racecar found!", *GetOwner()->GetName());
}


// Called every frame
void URacecarTimingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void URacecarTimingComponent::StartTimingSector(const float FromThisTime)
{
	InterSectorTiming = FromThisTime;
}

float URacecarTimingComponent::GetLastSectorDeltaTime() const
{
	CNULL_RET_EF(Racecar, "URacecarTimingComponent::GetLastSectorDeltaTime(): Racecar = nullptr!", 0.f);

	if (UWorld* World = Racecar->GetWorld())
	{
		return (float)World->TimeSince(InterSectorTiming);
	}

	E("URacecarTimingComponent::GetLastSectorDeltaTime(): GetWorld() = nullptr");
	return 0.f;
}

void URacecarTimingComponent::RegisterDeltaTime(const ARacepacePlayer* RacepacePlayer)
{
	if (Racecar)
	{
		if (RacepacePlayer)
		{
			if (URacecarTimingComponent* TimingComponent = Racecar->TimingComponent)
			{
				const float LastSectorDeltaTime = TimingComponent->GetLastSectorDeltaTime();
				if (DeltaTimes.Contains(Racecar))
				{
					DeltaTimes[Racecar].Add(LastSectorDeltaTime);
				}
				else
				{
					TArray<float> NewTime;
					NewTime.Add(LastSectorDeltaTime);
					DeltaTimes.Add(Racecar, NewTime);
				}
			}
		}
	}
}

void URacecarTimingComponent::RegisterLapCrossTime(const ARacepacePlayer* RacepacePlayer)
{
	if (Racecar)
	{
		if (RacepacePlayer)
		{
			const float CurrentLapTime = RacepacePlayer->GetCurrentLapTime();
			if (LapCrossTimes.Contains(Racecar))
			{
				LapCrossTimes[Racecar].Add(CurrentLapTime);
			}
			else
			{
				TArray<float> NewTime;
				NewTime.Add(CurrentLapTime);
				LapCrossTimes.Add(Racecar, NewTime);
			}
		}
	}
}

TArray<float> URacecarTimingComponent::GetFloatValues(TMap<ARacecar*, TArray<float>> TimeArray) const
{
	if (Racecar)
	{
		return TimeArray[Racecar];
	}

	return TArray<float>();
}
