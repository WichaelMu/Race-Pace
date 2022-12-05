// Fill out your copyright notice in the Description page of Project Settings.


#include "Racecar.h"
#include "RacePaceHelpers.h"

#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"

#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "WheeledVehicleMovementComponent4W.h"
#include "VehicleWheel.h"
#include "VehicleAnimInstance.h"

#include "TimerManager.h"
#include "Engine/EngineTypes.h"

#include "RacecarUIController.h"
#include "LapTimer.h"
#include "DashboardHUD.h"
#include "PersonalisedColours.h"

#define SHOW_ENGINE_ONSCREEN_MESSAGES 0 && UE_BUILD_DEVELOPMENT && WITH_EDITOR

#define ADD_FORWARD_GEAR(Engine, Gear, InRatio) \
Engine->TransmissionSetup.ForwardGears[Gear].Ratio = InRatio; \


PRAGMA_DISABLE_DEPRECATION_WARNINGS

ARacecar::ARacecar(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	PersonalisedColourComponent = CreateDefaultSubobject<UPersonalisedColours>(TEXT("Personalised Colours"));

	SpringArm->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	SpringArm->SetupAttachment(RootComponent);
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> DefaultChassis(TEXT("/Game/Ferzor/Ferzor"));
	if (DefaultChassis.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(DefaultChassis.Object);
	}
	else
	{
		E("Failed to find DefaultChassis")
	}

	static ConstructorHelpers::FClassFinder<UVehicleAnimInstance> DefaultAnim(TEXT("/Game/Ferzor/AB_Ferzor"));
	if (DefaultAnim.Succeeded())
	{
		GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		GetMesh()->SetAnimClass(DefaultAnim.Class);
	}

	SpringArm->TargetArmLength = 1000.f;
	SpringArm->SetRelativeLocation(FVector(-34.4f, 0, 80.f));
	SpringArm->SetRelativeRotation(FRotator(-20.f, 0, 0.f));
	SpringArm->bEnableCameraLag = true;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraLagSpeed = 5.f;
	SpringArm->CameraRotationLagSpeed = 5.f;
	SpringArm->CameraLagMaxDistance = 5.f;
	MouseMoveSensitivity = 5.f;
	ScrollZoomSensitivity = 2.f;

	Engine = CastChecked<UWheeledVehicleMovementComponent4W>(GetVehicleMovement());
	Engine->bReverseAsBrake = false;
	Engine->WheelSetups.Empty();
	Engine->WheelSetups.SetNum(4);
	Engine->WheelSetups[0].BoneName = FName(TEXT("FL"));
	Engine->WheelSetups[1].BoneName = FName(TEXT("FR"));
	Engine->WheelSetups[2].BoneName = FName(TEXT("RL"));
	Engine->WheelSetups[3].BoneName = FName(TEXT("RR"));
	Engine->WheelSetups[2].bDisableSteering = true;
	Engine->WheelSetups[3].bDisableSteering = true;

	static ConstructorHelpers::FClassFinder<UVehicleWheel> DefaultFront(TEXT("/Game/Ferzor/BP_FZ_Front"));
	static ConstructorHelpers::FClassFinder<UVehicleWheel> DefaultRear(TEXT("/Game/Ferzor/BP_FZ_Rear"));
	if (DefaultFront.Succeeded())
	{
		Engine->WheelSetups[0].WheelClass = DefaultFront.Class;
		Engine->WheelSetups[1].WheelClass = DefaultFront.Class;
	}

	if (DefaultRear.Succeeded())
	{
		Engine->WheelSetups[2].WheelClass = DefaultRear.Class;
		Engine->WheelSetups[3].WheelClass = DefaultRear.Class;
	}

	Engine->ChassisWidth = 130.f;
	Engine->ChassisHeight = 30.f;

	Engine->EngineSetup.MaxRPM = 12000.f;
	Engine->MaxEngineRPM = 12000.f;

	Engine->DifferentialSetup.DifferentialType = EVehicleDifferential4W::LimitedSlip_RearDrive;

	Engine->TransmissionSetup.bUseGearAutoBox = false;
	Engine->TransmissionSetup.GearSwitchTime = .1f;
	Engine->TransmissionSetup.GearAutoBoxLatency = 1.f;
	Engine->TransmissionSetup.ClutchStrength = 20.f;
	Engine->TransmissionSetup.FinalRatio = 1.f;
	Engine->TransmissionSetup.ReverseGearRatio = -23.f;
	Engine->TransmissionSetup.ClutchStrength = 560.f;

	Engine->EngineSetup.TorqueCurve.GetRichCurve()->Reset();
	Engine->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(0.f, 100.f);
	Engine->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(4200.f, 300.f);
	Engine->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(7000.f, 490.f);
	Engine->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(11500.f, 415.f);
	Engine->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(12000.f, 135.f);
	Engine->EngineSetup.DampingRateZeroThrottleClutchEngaged = .44f;

	Engine->SteeringCurve.GetRichCurve()->Reset();
	Engine->SteeringCurve.GetRichCurve()->AddKey(0.f, 1.f);
	Engine->SteeringCurve.GetRichCurve()->AddKey(132.f, .3f);
	Engine->SteeringCurve.GetRichCurve()->AddKey(290.f, .1f);

	BaseMass = 750.f;
	Engine->Mass = BaseMass;

	RacecarUIController = CreateDefaultSubobject<URacecarUIController>("Racecar UI Controller");

	LapTimingComponent = CreateDefaultSubobject<ULapTimer>("Lap Timing Component");
	DashboardHUDComponent = CreateDefaultSubobject<UDashboardHUD>("Dashboard HUD Component");

	LapTimingComponent->RacecarUIController = RacecarUIController;
	LapTimingComponent->Racecar = this;
	DashboardHUDComponent->RacecarUIController = RacecarUIController;
	DashboardHUDComponent->Racecar = this;
}


void ARacecar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Throttle", this, &ARacecar::Throttle);
	PlayerInputComponent->BindAxis("Brake", this, &ARacecar::Brake);
	PlayerInputComponent->BindAxis("Steer", this, &ARacecar::Steer);

	PlayerInputComponent->BindAxis("LookUp", this, &ARacecar::LookUp);
	PlayerInputComponent->BindAxis("LookRight", this, &ARacecar::LookRight);

	PlayerInputComponent->BindAxis("Scroll", this, &ARacecar::AdjustZoom);

	PlayerInputComponent->BindAction("Gear Up", IE_Pressed, this, &ARacecar::ShiftUp);
	PlayerInputComponent->BindAction("Gear Down", IE_Pressed, this, &ARacecar::ShiftDown);
}


void ARacecar::Throttle(float Throw)
{
	GetVehicleMovementComponent()->SetHandbrakeInput(false);

	if (Throw > 0)
	{
		GetVehicleMovementComponent()->SetThrottleInput(Throw);
	}
	else if (GetSpeed() == 0 && GetCurrentGear() != 0)
	{
		// Idle Revs.
		GetVehicleMovementComponent()->SetHandbrakeInput(true);
		GetVehicleMovementComponent()->SetThrottleInput(1.f);
	}
	else
	{
		GetVehicleMovementComponent()->SetThrottleInput(0.f);
	}
}

void ARacecar::Brake(float Throw)
{
	GetVehicleMovementComponent()->SetBrakeInput(Throw);

	if (PersonalisedColourComponent)
	{
		PersonalisedColourComponent->ActivateColour(Throw != 0.f, 0);
	}
}

void ARacecar::Steer(float Throw)
{
	GetVehicleMovementComponent()->SetSteeringInput(Throw);

	if (bAcceptRollCorrection)
	{
		const FRotator Rotation = GetActorRotation();
		if (FMath::Abs(Rotation.Roll) > 135.f)
		{
			if (UPrimitiveComponent* Physics = Engine->UpdatedPrimitive)
			{
				if (GetWorld())
				{
					const float RollForce = -30.f;

					const FVector SteeringInput = FVector(Throw * RollForce, 0.f, 0.f) * GetWorld()->GetDeltaSeconds() * 100.f;
					const FVector AngularRotationRoll = Rotation.RotateVector(SteeringInput);

					Physics->SetPhysicsAngularVelocity(AngularRotationRoll, true);
				}
			}
		}
	}
}

void ARacecar::LookUp(float Throw)
{
	if (Throw != 0)
	{
		FRotator NewRotation = SpringArm->GetRelativeRotation();
		NewRotation.Pitch += Throw * MouseMoveSensitivity;
		SpringArm->SetRelativeRotation(NewRotation);
	}
}

void ARacecar::LookRight(float Throw)
{
	if (Throw != 0)
	{
		FRotator NewRotation = SpringArm->GetRelativeRotation();
		NewRotation.Yaw += Throw * MouseMoveSensitivity;
		SpringArm->SetRelativeRotation(NewRotation);
	}
}

void ARacecar::AdjustZoom(float Throw)
{
	SpringArm->TargetArmLength += Throw * ScrollZoomSensitivity;
}

void ARacecar::ShiftDown()
{
	int32 CurrentGear = GetCurrentGear();

	// Prevent engaging Reverse when still going forward at Speed. (Stops instantly stopping because of Reverse)
	if (CurrentGear == 0 && GetSpeed() > 15)
	{
		return;
	}

	GetVehicleMovement()->SetTargetGear(ClampGear(CurrentGear - 1), true);
}

void ARacecar::ShiftUp()
{
	GetVehicleMovement()->SetTargetGear(ClampGear(GetCurrentGear() + 1), true);
}

int32 ARacecar::GetSpeed() const
{
	const float Speed = Engine->GetForwardSpeed() * .036f;

#if SHOW_ENGINE_ONSCREEN_MESSAGES
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(2, 0.f, FColor::White, FString::FromInt(Speed));
	}
#endif

	const int32 RoundedSpeed = FMath::RoundToInt(Speed);
	return FMath::Abs(RoundedSpeed);
}

int32 ARacecar::GetRPM() const
{
	const int32 RPM = FMath::RoundToInt(Engine->GetEngineRotationSpeed());
#if SHOW_ENGINE_ONSCREEN_MESSAGES
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 0.f, FColor::Red, FString::FromInt(RPM));
	}
#endif

	return RPM;
}

int32 ARacecar::GetCurrentGear() const
{
	const int32 Gear = GetVehicleMovement()->GetCurrentGear();
#if SHOW_ENGINE_ONSCREEN_MESSAGES
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(0, 0.f, FColor::Blue, FString::FromInt(Gear));
	}
#endif

	return Gear;
}

int32 ARacecar::ClampGear(const int32& Gear) const
{
	int32 Max = Engine->TransmissionSetup.ForwardGears.Num() - 1;
	return FMath::Clamp<int32>(Gear, -1, Max);
}

PRAGMA_ENABLE_DEPRECATION_WARNINGS
