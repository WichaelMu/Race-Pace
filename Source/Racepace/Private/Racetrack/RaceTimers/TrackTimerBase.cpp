// Fill out your copyright notice in the Description page of Project Settings.


#include "Racetrack/RaceTimers/TrackTimerBase.h"
#include "Engine/World.h"
#include "Racecar.h"

// Sets default values
ATrackTimerBase::ATrackTimerBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ATrackTimerBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATrackTimerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

IMPLEMENT_ENTER_FUNCTION(ATrackTimerBase)
{
	if (const ARacecar* Racecar = CastToRacecar(OtherActor))
	{
		if (UWorld* World = GetWorld())
		{
			EnterTime = World->GetTimeSeconds();
		}
	}
}

