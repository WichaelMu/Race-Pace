// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Racetrack/RacetrackEvent.h"
#include "TrackTimerBase.generated.h"

UCLASS()
class RACEPACE_API ATrackTimerBase : public ARacetrackEvent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrackTimerBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	OVERRIDE_ENTER_FUNCTION()


protected:

	double EnterTime;

};
