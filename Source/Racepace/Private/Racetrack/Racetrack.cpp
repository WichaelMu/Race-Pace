// Fill out your copyright notice in the Description page of Project Settings.


#include "Racetrack/Racetrack.h"

// Sets default values
ARacetrack::ARacetrack()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ARacetrack::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARacetrack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

