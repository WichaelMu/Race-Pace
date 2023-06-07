// Fill out your copyright notice in the Description page of Project Settings.


#include "Racetrack/RacetrackEvent.h"
#include "Components/BoxComponent.h"
#include "Racecar.h"

// Sets default values
ARacetrackEvent::ARacetrackEvent()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Collider = CreateDefaultSubobject<UBoxComponent>("Collider");
	Collider->OnComponentBeginOverlap.AddDynamic(this, &ARacetrackEvent::Enter);
}

// Called when the game starts or when spawned
void ARacetrackEvent::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARacetrackEvent::Enter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

const ARacecar* ARacetrackEvent::CastToRacecar(const AActor* OtherActor) const
{
	return Cast<ARacecar>(OtherActor);
}

