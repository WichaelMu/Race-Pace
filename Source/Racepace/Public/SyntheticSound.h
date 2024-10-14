// Racepace is a game built by WichaelMu.

#pragma once

#include "CoreMinimal.h"
#include "Sound/SoundWaveProcedural.h"
#include "SyntheticSound.generated.h"


UENUM(BlueprintType)
enum class EEngineType : uint8
{
	None       UMETA(DisplayName = "None"), 
	ICEV8Low   UMETA(DisplayName = "ICE V8 Low"), 
	ICEV8High  UMETA(DisplayName = "ICE V8 High"), 
	ICEV10Low  UMETA(DisplayName = "ICE V10 Low"), 
	ICEV10High UMETA(DisplayName = "ICE V10 High"),
	Electric   UMETA(DisplayName = "Electric"),
};


/**
 * 
 */
UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class RACEPACE_API USyntheticSound : public USoundWaveProcedural
{
	GENERATED_BODY()


public:

	USyntheticSound(const FObjectInitializer& ObjectInitializer);
	

public:

	UFUNCTION(BlueprintCallable, Category = "CarEngineSound")
		void SetEngineType(EEngineType EngineType, bool bIsTurbocharged);

	UFUNCTION(BlueprintCallable, Category = "CarEngineSound")
		void SetRPM(float NewRPM);

	UFUNCTION(BlueprintCallable, Category = "CarEngineSound")
		void SetThrottle(float ThrottleAmount);


protected:

	virtual int32 GeneratePCMData(uint8* PCMData, const int32 SamplesNeeded) override;

public:

	UPROPERTY(EditDefaultsOnly, Category = "CarEngineSound")
		EEngineType CurrentEngineType;

private:

	bool bRacecarIsTurbo;
	float RacecarRPM;
	float RacecarThrottle;

	float EnginePhase;
	float EngineFrequency;
	float EngineTurboFactor;

	// Helper functions for generating different engine types
	void GenerateEngineSound(float& OutSample, float Frequency, float ThrottleAmount);
	void AddTurboWhine(float& OutSample, float TurboFactor);

};
