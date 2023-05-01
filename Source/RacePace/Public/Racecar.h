// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "Racecar.generated.h"

class USpringArmComponent;
class UCameraComponent;
class USceneComponent;
class UChaosWheeledVehicleMovementComponent;

class ARacepacePlayer;
class URacecarUIController;
class UDashboard;

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
	UChaosWheeledVehicleMovementComponent* Engine;

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
};
