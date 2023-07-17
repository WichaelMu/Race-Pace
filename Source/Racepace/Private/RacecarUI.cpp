// Fill out your copyright notice in the Description page of Project Settings.


#include "RacecarUI.h"
#include "Racecar.h"
#include "RacepacePlayer.h"

// Sets default values for this component's properties
URacecarUI::URacecarUI()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void URacecarUI::BeginPlay()
{
	Super::BeginPlay();

	// ...

}


// Called every frame
void URacecarUI::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void URacecarUI::SetPlayer(ARacepacePlayer* InPlayer)
{
	Player = InPlayer;
}

ARacepacePlayer* URacecarUI::GetPlayer()
{
	if (Player)
	{
		return Player;
	}

	if (Racecar)
	{
		if (ARacepacePlayer* RacepacePlayer = Cast<ARacepacePlayer>(Racecar->GetController()))
		{
			Player = RacepacePlayer;
		}
	}

	return Player;
}

void URacecarUI::DecoratedLapTime(const float RawLapTime, int32& Minutes, int32& Seconds, int32& Milliseconds) const
{
	Seconds = (int)RawLapTime;

	Milliseconds = (RawLapTime - Seconds) * 1000;

	Minutes = (int)(Seconds / 60);

	Seconds %= 60;
}

FString URacecarUI::DecoratedLapTime(const float RawLapTime) const
{
	int32 Minutes, Seconds, Milliseconds;
	DecoratedLapTime(RawLapTime, Minutes, Seconds, Milliseconds);

	return FString::Printf(TEXT("%i:%02i:%03i"), Minutes, Seconds, Milliseconds);
}

