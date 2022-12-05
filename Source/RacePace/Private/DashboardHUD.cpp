// Fill out your copyright notice in the Description page of Project Settings.


#include "DashboardHUD.h"

#include "Racecar.h"
#include "RacePacePlayer.h"
#include "RacecarUIController.h"

// Sets default values for this component's properties
UDashboardHUD::UDashboardHUD()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called every frame
void UDashboardHUD::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	if (Racecar && RacecarUIController)
	{
		RacecarUIController->SetSpeed(Racecar->GetSpeed());
		RacecarUIController->SetRPM(Racecar->GetRPM());

		int32 Gear = Racecar->GetCurrentGear();
		FString GearAsString;

		if (Gear > 0)
		{
			GearAsString = FString::Printf(TEXT("%i"), Gear);
		}
		else if (Gear == 0)
		{
			GearAsString = FString("N");
		}
		else if (Gear == -1)
		{
			GearAsString = FString("R");
		}

		RacecarUIController->SetGear(GearAsString);
	}
}

