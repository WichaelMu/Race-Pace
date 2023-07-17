// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Racetrack.generated.h"

class AStartFinishLine;
class ASector;

UCLASS()
class RACEPACE_API ARacetrack : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARacetrack();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:

	UPROPERTY(EditInstanceOnly)
		TSubclassOf<AStartFinishLine> StartLine;
	UPROPERTY(EditInstanceOnly)
		TSubclassOf<AStartFinishLine> FinishLine;

	UPROPERTY(EditInstanceOnly)
		TArray<ASector*> Sectors;
};

UENUM()
enum ERacetrackLapType
{
	NotStarted,
	Hotlap,
	Inlap
};
