// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "Racecar.generated.h"

class USpringArmComponent;
class UCameraComponent;
class USceneComponent;

class ARacepacePlayer;
class URacecarUIController;
class UDashboard;

class UPersonalisedColours;

#define CHAOS_VEHICLE(Racecar) Cast<UChaosWheeledVehicleMovementComponent>((Racecar)->GetVehicleMovement())

/**
 *
 */
UCLASS()
class RACEPACE_API ARacecar : public AWheeledVehiclePawn
{
	GENERATED_BODY()

public:

	ARacecar(const FObjectInitializer& ObjectInitializer);

	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;


public:

	UDashboard* Dashboard;
	ARacepacePlayer* RacepacePlayer;

	UPROPERTY(EditAnywhere, Category = UI)
		URacecarUIController* RacecarUIController;

public:

	ARacepacePlayer* GetRacepacePlayerController();
	FORCEINLINE void StartLap(const float StartTime);
	FORCEINLINE void StopLap();

	UFUNCTION(BlueprintCallable)
		int32 GetSpeed() const;
	int32 GetRPM() const;
	int32 GetGear(bool bGetTargetGearInstead = false) const;
	FString GetGearString(bool bGetTargetGearInstead = false) const;

public:

	FORCEINLINE bool HasLapStarted(float& LapTime) const;

private:

	UFUNCTION(BlueprintCallable)
		void ShiftUp();
	UFUNCTION(BlueprintCallable)
		void ShiftDown();

	void Throttle(float Throw);
	void Brake(float Throw);
	void Steer(float Throw);

	void LookUp(float Throw);
	void LookRight(float Throw);

	void ZoomCamera(float Throw);

	int32 ClampGear(const int32 Gear) const;

	void CheckReverseLights(const int32 TargetGear);

private:

	UPROPERTY(EditDefaultsOnly, Category = Setup)
		USpringArmComponent* SpringArm;
	UPROPERTY(EditDefaultsOnly, Category = Setup)
		UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly, Category = Setup)
		USceneComponent* Gimbal;

	UPROPERTY(EditDefaultsOnly)
		float MouseMoveSensitivity;
	UPROPERTY(EditDefaultsOnly)
		float MouseScrollSensitivity;
	float DesiredArmLength;

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "0.0", UIMin = "0.0", ClampMax = "1.0", UIMax = "1.0"))
		float EngineIdleThrottleInput;

	UPROPERTY(EditAnywhere, Category = Personalisation)
		UPersonalisedColours* PersonalisedColours;

	UPROPERTY(EditDefaultsOnly)
		FRuntimeFloatCurve AntiLockBrakingCurve;

	float LapStartTime;
	bool bLapHasStarted;
};
