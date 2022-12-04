// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IRacecarUI.h"

#include "LapTimer.generated.h"

#define USE_DELTATIMING 0


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RACEPACE_API ULapTimer : public UIRacecarUI
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULapTimer();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:

	void BeginLap();
	void EndLap();

	float GetLastLapTime() const;
	void ClearLapTimes();

	UPROPERTY(VisibleAnywhere, Category="Lap Timer")
		TArray<float> LapTimes;

	UPROPERTY(VisibleAnywhere, Category="Lap Timer")
		bool bIsCurrentlyOnHotlap;

	static void ParseTime(const float& RawTime, int32& Minutes, int32& Seconds, int32& Milliseconds);

	float GetCurrentLapTime() const;
	FString GetTime(const float& InTime) const;

private:

	float ElapsedLapTime;
	float BestLapTime;

#if !USE_DELTATIMING
	double StartRawTime;
#endif
};
