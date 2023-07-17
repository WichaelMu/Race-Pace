// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RacetrackEvent.h"
#include "Speedtrap.generated.h"

class ARacecar;

UCLASS()
class RACEPACE_API ASpeedtrap : public ARacetrackEvent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpeedtrap();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:

	OVERRIDE_ENTER_FUNCTION()

private:

	void PrintSpeedTrap(const ARacecar* Racecar);

private:

	int32 Key;

};
