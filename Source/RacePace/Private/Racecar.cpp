// Fill out your copyright notice in the Description page of Project Settings.


#include "Racecar.h"

#include "UObject/ConstructorHelpers.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "ChaosWheeledVehicleMovementComponent.h"
#include "ChaosVehicleWheel.h"
#include "VehicleAnimationInstance.h"

#include "RacepacePlayer.h"
#include "Dashboard.h"
#include "RacecarUIController.h"

#include "PersonalisedColours.h"

#include "RDefinitions.h"

#define SHOW_ENGINE_ONSCREEN_MESSAGES 0 && WITH_EDITOR

#define ADD_GEAR_RATIO(Ratio) Engine->TransmissionSetup.ForwardGearRatios.Add(Ratio)
#define ADD_TORQUE_CURVE(RPM, Torque) Engine->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(RPM, Torque)
#define ADD_STEERING_CURVE(KmpH, Ratio) Engine->SteeringSetup.SteeringCurve.GetRichCurve()->AddKey(KmpH, Ratio)

ARacecar::ARacecar(const FObjectInitializer& ObjectInitializer)
{
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	Gimbal = CreateDefaultSubobject<USceneComponent>(TEXT("Gimbal"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	Gimbal->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	SpringArm->SetupAttachment(Gimbal, NAME_None);
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	DesiredArmLength = 1000.f;
	SpringArm->TargetArmLength = DesiredArmLength;
	SpringArm->SetRelativeLocation(FVector(0.f, 0.f, 100.f));
	SpringArm->SetRelativeRotation(FRotator(-20.f, 0, 0.f));
	SpringArm->bEnableCameraLag = true;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraLagSpeed = 5.f;
	SpringArm->CameraRotationLagSpeed = 5.f;
	SpringArm->CameraLagMaxDistance = 5.f;
	SpringArm->bInheritRoll = false;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> DefaultChassis(TEXT("/Game/Ferzor/Ferzor"));
	if (DefaultChassis.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(DefaultChassis.Object);
	}

	static ConstructorHelpers::FClassFinder<UVehicleAnimationInstance> DefaultAnimation(TEXT("/Game/Ferzor/AB_Ferzor"));
	if (DefaultAnimation.Succeeded())
	{
		GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		GetMesh()->SetAnimClass(DefaultAnimation.Class);
	}

	UChaosWheeledVehicleMovementComponent* Engine = CastChecked<UChaosWheeledVehicleMovementComponent>(GetVehicleMovement());
	Engine->bReverseAsBrake = false;
	Engine->WheelSetups.Empty();
	Engine->WheelSetups.SetNum(4);
	Engine->WheelSetups[0].BoneName = FName(TEXT("FL"));
	Engine->WheelSetups[1].BoneName = FName(TEXT("FR"));
	Engine->WheelSetups[2].BoneName = FName(TEXT("RL"));
	Engine->WheelSetups[3].BoneName = FName(TEXT("RR"));

	static ConstructorHelpers::FClassFinder<UChaosVehicleWheel> DefaultFront(TEXT("/Game/Ferzor/ChaosWheel_Ferzor_F"));
	static ConstructorHelpers::FClassFinder<UChaosVehicleWheel> DefaultRear(TEXT("/Game/Ferzor/ChaosWheel_Ferzor_R"));
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

	Engine->DifferentialSetup.DifferentialType = EVehicleDifferential::RearWheelDrive;

	Engine->TransmissionSetup.bUseAutomaticGears = false;
	Engine->TransmissionSetup.GearChangeTime = .1f;
	Engine->TransmissionSetup.FinalRatio = 1.f;
	Engine->TransmissionSetup.ForwardGearRatios.Empty();
	ADD_GEAR_RATIO(26.f);
	ADD_GEAR_RATIO(19.f);
	ADD_GEAR_RATIO(15.f);
	ADD_GEAR_RATIO(10.f);
	ADD_GEAR_RATIO(7.775f);
	ADD_GEAR_RATIO(6.f);
	ADD_GEAR_RATIO(4.9f);
	ADD_GEAR_RATIO(3.8f);
	Engine->TransmissionSetup.ReverseGearRatios.Empty();
	Engine->TransmissionSetup.ReverseGearRatios.Add(23.f);

	Engine->TransmissionSetup.ChangeUpRPM = 10000.f;
	Engine->TransmissionSetup.ChangeDownRPM = 4500.f;
	Engine->TransmissionSetup.GearChangeTime = .05f;
	Engine->TransmissionSetup.TransmissionEfficiency = .96f;

	Engine->EngineSetup.TorqueCurve.GetRichCurve()->Reset();
	ADD_TORQUE_CURVE(0.f, 150.f);
	ADD_TORQUE_CURVE(3200.f, 60.f);
	ADD_TORQUE_CURVE(6000.f, 65.f);
	ADD_TORQUE_CURVE(7850.f, 110.f);
	ADD_TORQUE_CURVE(9500.f, 120.f);
	ADD_TORQUE_CURVE(10500.f, 100.f);
	ADD_TORQUE_CURVE(12000.f, 20.f);
	Engine->EngineSetup.MaxTorque = 250.f;

	Engine->EngineSetup.EngineIdleRPM = 2300.f;
	Engine->EngineSetup.EngineBrakeEffect = .68f;
	Engine->EngineSetup.EngineRevUpMOI = 5.f;
	Engine->EngineSetup.EngineRevDownRate = 150.f;

	Engine->SteeringSetup.SteeringCurve.GetRichCurve()->Reset();
	ADD_STEERING_CURVE(0.f, 1.f);
	ADD_STEERING_CURVE(60.f, .42f);
	ADD_STEERING_CURVE(100.f, .1f);
	ADD_STEERING_CURVE(150.f, .2f);
	ADD_STEERING_CURVE(240.f, .1f);
	Engine->SteeringSetup.SteeringType = ESteeringType::Ackermann;

	Engine->DragCoefficient = .4f;
	Engine->DownforceCoefficient = 80.f;

	Engine->Mass = 750.f;

	if (UPrimitiveComponent* Physics = Cast<UPrimitiveComponent>(GetMesh()))
	{
		Physics->SetSimulatePhysics(true);
	}

	bUseSyntheticABS = true;
	AntiLockBrakingCurve.GetRichCurve()->AddKey(-80.f, .4f);
	AntiLockBrakingCurve.GetRichCurve()->AddKey(0.f, .2f);
	AntiLockBrakingCurve.GetRichCurve()->AddKey(20.f, 1.f);

	MouseMoveSensitivity = 5.f;
	MouseScrollSensitivity = 15.f;
	EngineIdleThrottleInput = .05f;

	RacecarUIController = CreateDefaultSubobject<URacecarUIController>(TEXT("Racecar UI Controller"));
	Dashboard = CreateDefaultSubobject<UDashboard>(TEXT("Dashboard HUD Component"));

	RacecarUIController->Racecar = Dashboard->Racecar = this;

	PersonalisedColours = CreateDefaultSubobject<UPersonalisedColours>(TEXT("Personalised Colours"));

	bIsElectric = false;
}

void ARacecar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// ...

	PlayerInputComponent->BindAction("Up Shift", EInputEvent::IE_Pressed, this, &ARacecar::ShiftUp);
	PlayerInputComponent->BindAction("Down Shift", EInputEvent::IE_Pressed, this, &ARacecar::ShiftDown);

	PlayerInputComponent->BindAxis("Throttle", this, &ARacecar::Throttle);
	PlayerInputComponent->BindAxis("Brake", this, &ARacecar::Brake);
	PlayerInputComponent->BindAxis("Steer", this, &ARacecar::Steer);

	PlayerInputComponent->BindAxis("HorizontalLook", this, &ARacecar::LookRight);
	PlayerInputComponent->BindAxis("VerticalLook", this, &ARacecar::LookUp);

	PlayerInputComponent->BindAxis("Zoom", this, &ARacecar::ZoomCamera);
}

void ARacecar::Throttle(float Throw)
{
	if (Throw == 0.f && EngineIdleThrottleInput != 0.f && GetSpeed() == 0)
	{
		Throw = EngineIdleThrottleInput;
	}

	GetVehicleMovement()->SetThrottleInput(Throw);
}

void ARacecar::Brake(float Throw)
{
	if (bUseSyntheticABS && Throw > .001f && GetSpeed() != 0)
	{
		int32 Kmph = GetSpeed(false);
		float BrakeInput = AntiLockBrakingCurve.GetRichCurve()->Eval(Kmph);
		Throw = FMath::Clamp(BrakeInput, 0.f, 1.f);
	}

	GetVehicleMovement()->SetBrakeInput(Throw);

	if (PersonalisedColours)
	{
		bool bThrowIsZero = Throw == 0.f;
		int32 Gear = GetGear();

		if (bThrowIsZero && Gear == -1)
		{
			PersonalisedColours->ActivateColour(false, 1);
			PersonalisedColours->ActivateColour(true, 0);
		}
		else
		{
			PersonalisedColours->ActivateColour(false, 0);
			PersonalisedColours->ActivateColour(!bThrowIsZero, 1);
		}
	}
}

void ARacecar::Steer(float Throw)
{
	GetVehicleMovementComponent()->SetSteeringInput(Throw);

	const FRotator Rotation = GetActorRotation();
	if (FMath::Abs(Rotation.Roll) > 135.f)
	{
		if (UPrimitiveComponent* Physics = GetVehicleMovement()->UpdatedPrimitive)
		{
			if (UWorld* World = GetWorld())
			{
				const float RollForce = -30.f;

				const FVector SteeringInput = FVector(Throw * RollForce, 0.f, 0.f) * World->GetDeltaSeconds() * 100.f;
				const FVector AngularRotationRoll = Rotation.RotateVector(SteeringInput);

				Physics->SetPhysicsAngularVelocityInDegrees(AngularRotationRoll, true);
			}
		}
	}
}

void ARacecar::LookUp(float Throw)
{
	if (Throw != 0.f)
	{
		FRotator NewRotation = SpringArm->GetRelativeRotation();
		NewRotation.Pitch += Throw * MouseMoveSensitivity;
		SpringArm->SetRelativeRotation(NewRotation);
	}
}

void ARacecar::LookRight(float Throw)
{
	if (Throw != 0.f)
	{
		FRotator NewRotation = Gimbal->GetComponentRotation();
		NewRotation.Yaw += Throw * MouseMoveSensitivity;
		Gimbal->SetWorldRotation(NewRotation);
	}
}

void ARacecar::ZoomCamera(float Throw)
{
	if (Throw != 0.f)
	{
		DesiredArmLength += Throw * MouseScrollSensitivity;
	}

	float MaxArmLength = FMath::Max(DesiredArmLength, 1.f);
	SpringArm->TargetArmLength = FMath::Lerp(SpringArm->TargetArmLength, MaxArmLength, GetWorld()->GetDeltaSeconds() * 3.f);
}

ARacepacePlayer* ARacecar::GetRacepacePlayerController()
{
	if (!RacepacePlayer)
	{
		if (ARacepacePlayer* RacepaceController = Cast<ARacepacePlayer>(GetController()))
		{
			RacepacePlayer = RacepaceController;
		}
	}

	return RacepacePlayer;
}


void ARacecar::ShiftUp()
{
	int32 CurrentGear = GetGear();
	int32 TargetGear = ClampGear(CurrentGear + 1);
	GetVehicleMovement()->SetTargetGear(ClampGear(TargetGear), true);
	RacecarUIController->SetGear(GetGearString(true));

	CheckReverseLights(GetGear(true));
}

void ARacecar::ShiftDown()
{
	if (GetGear() > 1 && GetRPM() >= CHAOS_VEHICLE(this)->TransmissionSetup.ChangeUpRPM)
	{
		return;
	}

	int32 CurrentGear = GetGear();
	int32 TargetGear = ClampGear(CurrentGear - 1);
	GetVehicleMovement()->SetTargetGear(ClampGear(TargetGear), true);
	RacecarUIController->SetGear(GetGearString(true));

	CheckReverseLights(GetGear(true));
}

int32 ARacecar::GetSpeed(bool bGetAbsolute) const
{
	const float Speed = GetVehicleMovement()->GetForwardSpeed() * .036f;

#if SHOW_ENGINE_ONSCREEN_MESSAGES
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(2, 0.f, FColor::White, FString::FromInt(Speed));
	}
#endif

	const int32 RoundedSpeed = FMath::RoundToInt(Speed);
	return bGetAbsolute ? FMath::Abs(RoundedSpeed) : RoundedSpeed;
}

int32 ARacecar::GetRPM() const
{
	const int32 RPM = FMath::RoundToInt(CHAOS_VEHICLE(this)->GetEngineRotationSpeed());

#if SHOW_ENGINE_ONSCREEN_MESSAGES
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 0.f, FColor::Red, FString::FromInt(RPM));
	}
#endif

	return RPM;
}

int32 ARacecar::GetGear(bool bGetTargetGearInstead) const
{
	const int32 Gear = bGetTargetGearInstead
		? GetVehicleMovement()->GetTargetGear()
		: GetVehicleMovement()->GetCurrentGear();

#if SHOW_ENGINE_ONSCREEN_MESSAGES
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(0, 0.f, FColor::Blue, FString::FromInt(Gear));
	}
#endif

	return Gear;
}

FString ARacecar::GetGearString(bool bGetTargetGearInstead) const
{
	int32 Gear = GetGear(bGetTargetGearInstead);

	if (Gear == 0)
	{
		return FString::Printf(TEXT("N"));
	}

	if (Gear == -1)
	{
		return FString::Printf(TEXT("R"));
	}

	return FString::Printf(TEXT("%i"), Gear);
}

int32 ARacecar::ClampGear(const int32 Gear) const
{
	int32 Max = CHAOS_VEHICLE(this)->TransmissionSetup.ForwardGearRatios.Num() - 1;
	return FMath::Clamp<int32>(Gear, -1, Max);
}

void ARacecar::CheckReverseLights(const int32 TargetGear)
{
	PersonalisedColours->ActivateColour(TargetGear == -1, 0);
}
