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

