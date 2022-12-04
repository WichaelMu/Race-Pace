// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "IRacecarUI.generated.h"

class ARacecar;
class URacecarUIController;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RACEPACE_API UIRacecarUI : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UIRacecarUI();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


public:

	ARacecar* Racecar;
	URacecarUIController* RacecarUIController;
		
};
