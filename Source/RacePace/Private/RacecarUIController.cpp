// Fill out your copyright notice in the Description page of Project Settings.


#include "RacecarUIController.h"

#include "RDefinitions.h"

#include "RacePacePlayer.h"
#include "Racecar.h"
#include "ChaosWheeledVehicleMovementComponent.h"

#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanelSlot.h"

#include "Materials/MaterialInterface.h"
#include "Materials/MaterialInstanceDynamic.h"

// Sets default values for this component's properties
URacecarUIController::URacecarUIController()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

	static ConstructorHelpers::FClassFinder<UUserWidget> MainWidgetBlueprint(TEXT("/Game/Widgets/UI_PlayerHUD"));
	if (MainWidgetBlueprint.Succeeded())
	{
		DashboardHUDWidget = MainWidgetBlueprint.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> SectorWidgetBlueprint(TEXT("/Game/Widgets/UI_SectorSplit"));
	if (SectorWidgetBlueprint.Succeeded())
	{
		SectorSplitWidget = SectorWidgetBlueprint.Class;
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> DefaultLinearGradientMaterial(TEXT("/Game/Widgets/WidgetMaterials/MI_LinearGradient"));
	if (DefaultLinearGradientMaterial.Succeeded())
	{
		LinearGradientMaterial = DefaultLinearGradientMaterial.Object;
	}

	ShiftUpText = TEXT("SHIFT ^");
	RevUpRPM = 0.f;
}


// Called when the game starts
void URacecarUIController::BeginPlay()
{
	Super::BeginPlay();

	// ...

	if (ARacecar* RacecarPawn = Cast<ARacecar>(GetOwner()))
	{
		Racecar = RacecarPawn;
		if (ARacepacePlayer* PlayerController = Cast<ARacepacePlayer>(RacecarPawn->GetController()))
		{
			SetPlayer(PlayerController);
		}
	}

	ARacepacePlayer* RacepacePlayer = GetPlayer();

	TimingWidget = CreateWidget<UUserWidget>(GetWorld(), DashboardHUDWidget);
	TimingWidget->SetOwningPlayer(RacepacePlayer);

	if (UUserWidget* Widget = Cast<UUserWidget>(TimingWidget))
	{
		// Lap Time Assignments.
		TimingText = Cast<UTextBlock>(Widget->GetWidgetFromName(TEXT("TimingText")));
		BestTimeText = Cast<UTextBlock>(Widget->GetWidgetFromName(TEXT("BestTimeText")));
		LapDeltaText = Cast<UTextBlock>(Widget->GetWidgetFromName(TEXT("LapDeltaText")));
		LastLapTimeText = Cast<UTextBlock>(Widget->GetWidgetFromName(TEXT("LastLapTime")));

		// Dashboard Assignments.
		KMPHSpeedText = Cast<UTextBlock>(Widget->GetWidgetFromName(TEXT("KMPHSpeedText")));
		GearText = Cast<UTextBlock>(Widget->GetWidgetFromName(TEXT("GearText")));
		RPMText = Cast<UTextBlock>(Widget->GetWidgetFromName(TEXT("RPMText")));
		RPMCurrent = Cast<UImage>(Widget->GetWidgetFromName(TEXT("RPM_Current")));
		RPMBlocks.Add(Cast<UImage>(Widget->GetWidgetFromName(TEXT("RPM_Bar_1"))));
		RPMBlocks.Add(Cast<UImage>(Widget->GetWidgetFromName(TEXT("RPM_Bar_2"))));
		RPMBlocks.Add(Cast<UImage>(Widget->GetWidgetFromName(TEXT("RPM_Bar_3"))));
		RPMBlocks.Add(Cast<UImage>(Widget->GetWidgetFromName(TEXT("RPM_Bar_4"))));
		RPMBlocks.Add(Cast<UImage>(Widget->GetWidgetFromName(TEXT("RPM_Bar_5"))));
		RPMBlocks.Add(Cast<UImage>(Widget->GetWidgetFromName(TEXT("RPM_Bar_6"))));
		RPMBlocks.Add(Cast<UImage>(Widget->GetWidgetFromName(TEXT("RPM_Bar_7"))));

		RPMCurrentSlot = Cast<UCanvasPanelSlot>(RPMCurrent->Slot);
		RPMTextSlot = Cast<UCanvasPanelSlot>(RPMText->Slot);

		ShiftIndicator = Cast<UTextBlock>(Widget->GetWidgetFromName(TEXT("ShiftIndicator")));

		TimingWidget->AddToViewport();

		SetLinearGradients();
	}

	SectorWidget = CreateWidget<UUserWidget>(GetWorld(), SectorSplitWidget);
	SectorWidget->SetOwningPlayer(RacepacePlayer);

	if (UUserWidget* Widget = Cast<UUserWidget>(SectorWidget))
	{
		SectorTimingText = Cast<UTextBlock>(Widget->GetWidgetFromName(TEXT("SectorSplit")));

		SectorWidget->AddToViewport();
	}

	SetGear(Racecar->GetGearString());

	if (UChaosWheeledVehicleMovementComponent* Engine = CHAOS_VEHICLE(Racecar))
	{
		RevUpRPM = Engine->TransmissionSetup.ChangeUpRPM;
	}
}

void URacecarUIController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Racecar)
	{
		SetSpeed(Racecar->GetSpeed());
		const int32 RPM = Racecar->GetRPM();
		SetRPM(RPM);
		CalculateRPMGraphics(RPM, DeltaTime);

		if (ARacepacePlayer* RacepacePlayer = GetPlayer())
		{
			if (UWorld* World = GetWorld())
			{
				SetLapTime(RacepacePlayer->GetCurrentLapTime());
			}
		}
	}
}

void URacecarUIController::SetLinearGradients()
{
	for (UImage* Block : RPMBlocks)
	{
		UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(LinearGradientMaterial, nullptr);
		Block->SetBrushFromMaterial(DynamicMaterial);
	}
}

FString URacecarUIController::GetPrefixFromSplit(const float Split) const
{
	if (Split > 0.f)
	{
		return FString(TEXT("+"));
	}
	else if (Split < 0.f)
	{
		return FString(TEXT("-"));
	}

	return FString(TEXT("="));
}

////////////////////////////////////////////////////////////////////////////////
// Lap Time Implementations.
////////////////////////////////////////////////////////////////////////////////

void URacecarUIController::SetLapTime(const FString& InTime)
{
	if (TimingText)
	{
		TimingText->SetText(FText::FromString(InTime));
	}
}

void URacecarUIController::SetBestLapTime(const FString& InBestTime)
{
	if (BestTimeText)
	{
		BestTimeText->SetText(FText::FromString(InBestTime));
	}
}

void URacecarUIController::SetLastLapTime(const FString& InLastLapTime)
{
	if (LastLapTimeText)
	{
		LastLapTimeText->SetText(FText::FromString(InLastLapTime));
	}
}

void URacecarUIController::SetLapTime(const float InTime)
{
	SetLapTime(DecoratedLapTime(InTime));
}

void URacecarUIController::SetBestLapTime(const float InBestTime)
{
	SetBestLapTime(DecoratedLapTime(InBestTime));
}

void URacecarUIController::SetLastLapTime(const float InLastLapTime)
{
	SetLastLapTime(DecoratedLapTime(InLastLapTime));
}

void URacecarUIController::CompareLapToBestDeltas(const float& LapTime, const float& BestTime)
{
	if (LapDeltaText)
	{
		const float Delta = LapTime - BestTime;
		const float Sign = FMath::Sign(Delta);
		FString Result = FString("");

		const bool bIsBehind = Sign > 0.f;
		Result += bIsBehind
			? FString("+")
			: FString("-");
		Result += DecoratedLapTime(FMath::Abs(Delta));

		LapDeltaText->SetText(FText::FromString(Result));

		LapDeltaText->SetColorAndOpacity(bIsBehind
			? NegativeDeltaColour
			: PositiveDeltaGreen);
	}
}

void URacecarUIController::SetSplitTime(const float InSplit)
{
	SetSplitTime(DecoratedLapTime(InSplit));
}

void URacecarUIController::SetSplitTime(const FString& InSplit)
{
	if (SectorTimingText)
	{
		SectorTimingText->SetText(FText::FromString(InSplit));
	}
}

void URacecarUIController::DisplayDeltaSplitTimes(const float InDeltaSector, const float BestDeltaSector, const float InLapSector, const float BestLapSector)
{
	LF("Comparing InDeltaSector %f and BestDeltaSector %f", InDeltaSector, BestDeltaSector);
	LF("Comparing InLapSector %f and BestLapSector %f", InLapSector, BestLapSector);

	const float DeltaSplit = InDeltaSector - BestDeltaSector;
	const float LapSplit = InLapSector - BestLapSector;
	const FString DeltaPrefix = GetPrefixFromSplit(DeltaSplit);
	const FString LapPrefix = GetPrefixFromSplit(LapSplit);
	
	if (SectorTimingText)
	{
		const float AbsoluteDeltaSplit = FMath::Abs(DeltaSplit);
		FString DeltaSplitString = FString::Printf(TEXT("%s%s"), *DeltaPrefix, *DecoratedLapTime(AbsoluteDeltaSplit));

		const float AbsoluteLapSplit = FMath::Abs(LapSplit);
		FString LapSplitString = FString::Printf(TEXT("%s%s"), *LapPrefix, *DecoratedLapTime(AbsoluteLapSplit));

		FString FinalString = FString::Printf(TEXT("Sector Delta: %s\nLap Delta: %s"), *DeltaSplitString, *LapSplitString);
		FText FinalText = FText::FromString(FinalString);
		SectorTimingText->SetText(FinalText);
	}
}


////////////////////////////////////////////////////////////////////////////////
// Dashboard Implementations.
////////////////////////////////////////////////////////////////////////////////

void URacecarUIController::SetSpeed(const int32 InKMPH)
{
	if (KMPHSpeedText)
	{
		KMPHSpeedText->SetText(FText::FromString(FString::Printf(TEXT("%i KMpH"), InKMPH)));
	}
}

void URacecarUIController::SetRPM(const int32 InRPM)
{
	if (RPMText)
	{
		FString Suffix = Racecar->bIsElectric ? FString(TEXT("kWh")) : FString(TEXT("RPM"));
		RPMText->SetText(FText::FromString(FString::Printf(TEXT("%i %s"), InRPM, *Suffix)));
	}

	int32 Gear = Racecar->GetGear();
	if (ShiftIndicator && Gear != -1)
	{
		if (InRPM > RevUpRPM && Gear < CHAOS_VEHICLE(Racecar)->TransmissionSetup.ForwardGearRatios.Num() - 1)
		{
			ShiftIndicator->SetText(FText::FromString(ShiftUpText));
		}
		else
		{
			ShiftIndicator->SetText(FText::FromString(TEXT("")));
		}
	}
}

void URacecarUIController::SetGear(const FString InGear)
{
	if (GearText)
	{
		GearText->SetText(FText::FromString(InGear));
	}
}

void URacecarUIController::CalculateRPMGraphics(const int32 RPM, const float DeltaTime)
{
	if (!Racecar)
	{
		return;
	}

	if (!CHAOS_VEHICLE(Racecar))
	{
		return;
	}

	const float MaxRPMGraphicSize = 500.f;
	const int32 NumRPMBlocks = RPMBlocks.Num();
	const float RacecarMaxRPM = CHAOS_VEHICLE(Racecar)->EngineSetup.MaxRPM;

	const float CurrentToMaxRatio = RPM / RacecarMaxRPM;
	const float BlockPerRPMRange = RacecarMaxRPM / NumRPMBlocks;
	const float RPMGraphics = CurrentToMaxRatio * MaxRPMGraphicSize;

	UImage* DefaultBlock = RPMBlocks[0];
	UCanvasPanelSlot* DefaultPanelSlot = Cast<UCanvasPanelSlot>(DefaultBlock->Slot);
	const float DefaultHeight = DefaultPanelSlot->GetSize().Y;
	FVector2D RPMBarSize;

	for (int32 i = 0; i < NumRPMBlocks; ++i)
	{
		UImage* RPMBlock = RPMBlocks[i];

		if (float Function = CurveFunction(CurrentToMaxRatio, BlockPerRPMRange * (i + 1)) < RPM)
		{
			if (UMaterialInstanceDynamic* DynamicMaterial = RPMBlock->GetDynamicMaterial())
			{
				const float Transparency = CurrentToMaxRatio >= 1.f ? CurrentToMaxRatio - (1.f - CurrentToMaxRatio) : CurrentToMaxRatio;
				const float Scalar = 1.f - Transparency;
				DynamicMaterial->SetScalarParameterValue(FName("Scalar"), Scalar);
			}

			continue;
		}

		UCanvasPanelSlot* PanelSlot = Cast<UCanvasPanelSlot>(RPMBlock->Slot);

		RPMBarSize = FVector2D(RPMGraphics, DefaultHeight);
		PanelSlot->SetSize(RPMBarSize);
	}

	const float DefaultXPositionDefault = DefaultPanelSlot->GetPosition().X;
	const float RPMCurrentYDefault = RPMCurrentSlot->GetPosition().Y;
	FVector2D RPMToGraphicPosition = FVector2D(DefaultXPositionDefault + RPMBarSize.X, RPMCurrentYDefault);

	if (RPM <= RacecarMaxRPM)
	{
		FVector2D RPMInterp = FMath::Vector2DInterpTo(RPMCurrentSlot->GetPosition(), RPMToGraphicPosition, DeltaTime, 7.f);
		FVector2D RPMTextInterp;
		RPMTextInterp.X = RPMInterp.X;
		RPMTextInterp.Y = RPMTextSlot->GetPosition().Y;

		RPMTextSlot->SetPosition(RPMTextInterp);
		RPMCurrentSlot->SetPosition(RPMInterp);
	}
}


float URacecarUIController::CurveFunction(const float& Ratio, const float& Scalar)
{
	const float Function = 1.f + FMath::Pow(1.f - Ratio, 3.f);

	return Function * Scalar;
}