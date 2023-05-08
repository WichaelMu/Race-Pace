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

public:

	ARacepacePlayer* GetRacepacePlayerController();

	int32 GetSpeed() const;
	int32 GetRPM() const;
	int32 GetGear(bool bGetTargetGearInstead = false) const;
	FString GetGearString(bool bGetTargetGearInstead = false) const;

private:

	void ShiftUp();
	void ShiftDown();

	void Throttle(float Throw);
	void Brake(float Throw);
	void Steer(float Throw);

	void LookUp(float Throw);
	void LookRight(float Throw);

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

	URacecarUIController* RacecarUIController;

	UPROPERTY(EditAnywhere, Category = Personalisation)
		UPersonalisedColours* PersonalisedColours;
};
