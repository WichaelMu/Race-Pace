// Fill out your copyright notice in the Description page of Project Settings.


#include "Timer.h"
#include "Components/BoxComponent.h"
#include "RacepaceHelpers.h"

#include "Kismet/GameplayStatics.h"

#include "Racecar.h"
#include "RacePacePlayer.h"
#include "TimerManager.h"

#include "Engine/EngineTypes.h"
#include "Containers/Set.h"

#include "LapTimer.h"


// Sets default values
ATimer::ATimer()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	UBoxComponent* Collider = CreateDefaultSubobject<UBoxComponent>("Box Component");
	Collider->OnComponentBeginOverlap.AddDynamic(this, &ATimer::Enter);
	RootComponent = Collider;
}

// Called when the game starts or when spawned
void ATimer::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATimer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATimer::Enter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OnEntered.Broadcast(OtherActor);

	if (ARacecar* Racecar = Cast<ARacecar>(OtherActor))
	{
		if (ActiveLaps.Contains(Racecar))
		{
			Racecar->LapTimingComponent->EndLap();
			Racecar->LapTimingComponent->BeginLap();
		}
		else
		{
			Racecar->LapTimingComponent->BeginLap();
			ActiveLaps.Add(Racecar);
		}
	}
}
