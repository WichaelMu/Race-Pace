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

#define SHOW_ENGINE_ONSCREEN_MESSAGES 0

ARacecar::ARacecar(const FObjectInitializer& ObjectInitializer)
{
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	Gimbal = CreateDefaultSubobject<USceneComponent>(TEXT("Gimbal"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	Gimbal->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	SpringArm->SetupAttachment(Gimbal, NAME_None);
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	SpringArm->TargetArmLength = 1000.f;
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

	Engine = CastChecked<UChaosWheeledVehicleMovementComponent>(GetVehicleMovement());
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
	Engine->TransmissionSetup.ForwardGearRatios.Add(21.f);
	Engine->TransmissionSetup.ForwardGearRatios.Add(15.f);
	Engine->TransmissionSetup.ForwardGearRatios.Add(12.f);
	Engine->TransmissionSetup.ForwardGearRatios.Add(9.f);
	Engine->TransmissionSetup.ForwardGearRatios.Add(7.5f);
	Engine->TransmissionSetup.ForwardGearRatios.Add(4.f);
	Engine->TransmissionSetup.ForwardGearRatios.Add(2.f);
	Engine->TransmissionSetup.ForwardGearRatios.Add(1.35f);
	Engine->TransmissionSetup.ReverseGearRatios.Empty();
	Engine->TransmissionSetup.ReverseGearRatios.Add(23.f);

	Engine->TransmissionSetup.ChangeUpRPM = 10000.f;
	Engine->TransmissionSetup.ChangeDownRPM = 4500.f;
	Engine->TransmissionSetup.GearChangeTime = .05f;
	Engine->TransmissionSetup.TransmissionEfficiency = .96f;

	Engine->EngineSetup.TorqueCurve.GetRichCurve()->Reset();
	Engine->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(0.f, 100.f);
	Engine->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(7500.f, 250.f);
	Engine->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(11000.f, 200.f);
	Engine->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(12000.f, 130.f);
	Engine->EngineSetup.MaxTorque = 250.f;

	Engine->EngineSetup.EngineIdleRPM = 2300.f;
	Engine->EngineSetup.EngineRevUpMOI = 5.f;
	Engine->EngineSetup.EngineRevDownRate = 450.f;

	Engine->SteeringSetup.SteeringCurve.GetRichCurve()->Reset();
	Engine->SteeringSetup.SteeringCurve.GetRichCurve()->AddKey(0.f, 1.f);
	Engine->SteeringSetup.SteeringCurve.GetRichCurve()->AddKey(132.f, .3f);
	Engine->SteeringSetup.SteeringCurve.GetRichCurve()->AddKey(290.f, .1f);

	Engine->DragCoefficient = .4f;
	Engine->DownforceCoefficient = 80.f;

	Engine->Mass = 750.f;

	if (UPrimitiveComponent* Physics = Cast<UPrimitiveComponent>(GetMesh()))
	{
		Physics->SetSimulatePhysics(true);
	}

	MouseMoveSensitivity = 5.f;

	RacecarUIController = CreateDefaultSubobject<URacecarUIController>(TEXT("Racecar UI Controller"));
	Dashboard = CreateDefaultSubobject<UDashboard>(TEXT("Dashboard HUD Component"));

	RacecarUIController->Racecar = Dashboard->Racecar = this;

	PersonalisedColours = CreateDefaultSubobject<UPersonalisedColours>(TEXT("Personalised Colours"));
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
}

void ARacecar::Throttle(float Throw)
{
	GetVehicleMovement()->SetThrottleInput(Throw);
}

void ARacecar::Brake(float Throw)
{
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
		FRotator NewRotation = Gimbal->GetComponentRotation();
		NewRotation.Yaw += Throw * MouseMoveSensitivity;
		Gimbal->SetWorldRotation(NewRotation);
	}
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
	int32 CurrentGear = GetGear();
	int32 TargetGear = ClampGear(CurrentGear - 1);
	GetVehicleMovement()->SetTargetGear(ClampGear(TargetGear), true);
	RacecarUIController->SetGear(GetGearString(true));

	CheckReverseLights(GetGear(true));
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
	int32 Max = Engine->TransmissionSetup.ForwardGearRatios.Num() - 1;
	return FMath::Clamp<int32>(Gear, -1, Max);
}

void ARacecar::CheckReverseLights(const int32 TargetGear)
{
	PersonalisedColours->ActivateColour(TargetGear == -1, 0);
}
