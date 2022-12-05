// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehicle.h"
#include "Racecar.generated.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

class USkeletalMeshComponent;
class USpringArmComponent;
class UWheeledVehicleMovementComponent4W;
class UCameraComponent;

class URacecarUIController;
class ULapTimer;
class UDashboardHUD;
class UPersonalisedColours;

class UDashboard;

/**
 *
 */
UCLASS()
class RACEPACE_API ARacecar : public AWheeledVehicle
{
	GENERATED_BODY()

public:

	ARacecar(const FObjectInitializer& ObjectInitializer);

	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Engine)
		UWheeledVehicleMovementComponent4W* Engine;

	UPROPERTY(VisibleAnywhere)
		ULapTimer* LapTimingComponent;
	UPROPERTY(VisibleAnywhere)
		UDashboardHUD* DashboardHUDComponent;

	UPROPERTY(VisibleAnywhere)
		UPersonalisedColours* PersonalisedColourComponent;

	UFUNCTION(BlueprintCallable, Category = Dashboard)
		FORCEINLINE int32 GetRPM() const;
	UFUNCTION(BlueprintCallable, Category = Dashboard)
		FORCEINLINE int32 GetSpeed() const;
	UFUNCTION(BlueprintCallable, Category = Dashboard)
		FORCEINLINE int32 GetCurrentGear() const;

	UFUNCTION(BlueprintCallable)
		ARacePacePlayer* GetRacepacePlayerController();

private:

	void Throttle(float Throw);
	void Brake(float Throw);
	void Steer(float Throw);

	/** How much bias should be applied when steering? */
	UPROPERTY(EditAnywhere, Category = Steering)
		float RearDiffBias;
	
	void LookUp(float Throw);
	void LookRight(float Throw);

	void AdjustZoom(float Throw);

	void ShiftDown();
	void ShiftUp();


private:

	UPROPERTY(EditDefaultsOnly, Category = Setup)
		USkeletalMeshComponent* Chassis;

	UPROPERTY(EditDefaultsOnly, Category = Setup)
		USpringArmComponent* SpringArm;
	UPROPERTY(EditDefaultsOnly, Category = Setup)
		UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, Category = Camera)
		float MouseMoveSensitivity;
	UPROPERTY(EditAnywhere, Category = Camera)
		float ScrollZoomSensitivity;

	UPROPERTY(EditAnywhere, Category = RollCorrection)
		bool bAcceptRollCorrection;

	float BaseMass;

	URacecarUIController* RacecarUIController;
	ARacePacePlayer* RacepacePlayer;

private:

	int32 ClampGear(const int32& Gear) const;
};


PRAGMA_ENABLE_DEPRECATION_WARNINGS