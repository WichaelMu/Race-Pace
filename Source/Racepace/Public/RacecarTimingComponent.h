// Racepace is a game built by WichaelMu.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RacecarTimingComponent.generated.h"

class ARacecar;
class ARacepacePlayer;


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RACEPACE_API URacecarTimingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	URacecarTimingComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:

	FORCEINLINE void StartTimingSector(const float FromThisTime);
	FORCEINLINE float GetLastSectorDeltaTime() const;

	void RegisterDeltaTime(const ARacepacePlayer* RacepacePlayer);
	void RegisterLapCrossTime(const ARacepacePlayer* RacepacePlayer);

	TArray<float> GetFloatValues(TMap<ARacecar*, TArray<float>> TimeArray) const;

public:

	ARacecar* Racecar;

	// The delta-time between a Racecar passing the previous Sector to
	// this Sector.
	TMap<ARacecar*, TArray<float>> DeltaTimes;
	// The Lap-time this Racecar crossed this Sector.
	TMap<ARacecar*, TArray<float>> LapCrossTimes;

private:

	float InterSectorTiming;

};
