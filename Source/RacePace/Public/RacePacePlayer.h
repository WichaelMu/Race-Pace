// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "RacePacePlayer.generated.h"


class URacecarUIController;

/**
 * 
 */
UCLASS()
class RACEPACE_API ARacePacePlayer : public APlayerController
{
	GENERATED_BODY()

public:

	ARacePacePlayer();


protected:

	virtual void BeginPlay() override;


public:

	void AddLapTime(const float& NewLapTime);
	FORCEINLINE TArray<float> GetLapTimes() const;
	float GetLastLapTime() const;
	void ClearLapTimes();

private:

	UPROPERTY(VisibleAnywhere, Category="Lap Records")
		TArray<float> LapTimes;

};
