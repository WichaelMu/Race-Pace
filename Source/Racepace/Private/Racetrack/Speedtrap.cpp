// Fill out your copyright notice in the Description page of Project Settings.


#include "Racetrack/Speedtrap.h"
#include "Racecar.h"
#include "Engine/Engine.h"

// Sets default values
ASpeedtrap::ASpeedtrap()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Key = 0;
}

// Called when the game starts or when spawned
void ASpeedtrap::BeginPlay()
{
	Super::BeginPlay();

}

void ASpeedtrap::Enter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (const ARacecar* Racecar = CastToRacecar(OtherActor))
	{
		PrintSpeedTrap(Racecar);
	}
}

void ASpeedtrap::PrintSpeedTrap(const ARacecar* Racecar)
{
	if (GEngine)
	{
		FString Message = FString::Printf(TEXT("%s: Speed Trap: %i kmph"), *Racecar->GetName(), Racecar->GetSpeed());
		GEngine->AddOnScreenDebugMessage(Key++, 5.f, FColor::White, Message, false);
	}
}