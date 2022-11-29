// Fill out your copyright notice in the Description page of Project Settings.


#include "RacePacePlayer.h"
#include "RacePaceHelpers.h"

#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "LapTimer.h"

ARacePacePlayer::ARacePacePlayer()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> MainTimingWidget(TEXT("/Game/Widgets/UI_PlayerHUD"));

	if (MainTimingWidget.Succeeded())
	{
		TimingWidget = MainTimingWidget.Class;
	}
}

void ARacePacePlayer::SetLapTime(const FString& InTime)
{
	if (TimingText)
	{
		TimingText->SetText(FText::FromString(InTime));
	}
}

void ARacePacePlayer::SetBestLapTime(const FString& InBestTime)
{
	if (BestTimeText)
	{
		BestTimeText->SetText(FText::FromString(InBestTime));
	}
}

void ARacePacePlayer::SetLastLapTime(const FString& InLastLapTime)
{
	if (LastLapTimeText)
	{
		LastLapTimeText->SetText(FText::FromString(InLastLapTime));
	}
}

void ARacePacePlayer::CompareLapToBestDeltas(const float& LapTime, const float& BestTime, const ULapTimer* LapTimingComponent)
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
		Result += LapTimingComponent->GetTime(FMath::Abs(Delta));

		LapDeltaText->SetText(FText::FromString(Result));

		LapDeltaText->SetColorAndOpacity(bIsBehind
			? NegativeDeltaColour
			: PositiveDeltaGreen);
	}
}

void ARacePacePlayer::BeginPlay()
{
	Super::BeginPlay();

	// ...

	RacepaceTimingWidget = CreateWidget<UUserWidget>(GetWorld(), TimingWidget);
	if (APawn* SelfOwner = GetPawn())
	{
		if (APlayerController* PawnController = Cast<APlayerController>(SelfOwner->GetController()))
		{
			RacepaceTimingWidget->SetOwningPlayer(PawnController);
		}
	}

	if (UUserWidget* Widget = Cast<UUserWidget>(RacepaceTimingWidget))
	{
		TimingText = Cast<UTextBlock>(Widget->GetWidgetFromName(TEXT("TimingText")));
		if (TimingText)
		{
			FString String = FString("## EMPTY_STRING_PLEASE_IGNORE ##");
			TimingText->SetText(FText::FromString(String));
		}

		BestTimeText = Cast<UTextBlock>(Widget->GetWidgetFromName(TEXT("BestTimeText")));
		if (BestTimeText)
		{
			FString String = FString("## YOUR BEST TIME GOES HERE ##");
			BestTimeText->SetText(FText::FromString(String));
		}

		LapDeltaText = Cast<UTextBlock>(Widget->GetWidgetFromName(TEXT("LapDeltaText")));
		if (LapDeltaText)
		{
			FString String = FString("## THE LAP DELTA GOES HERE ##");
			LapDeltaText->SetText(FText::FromString(String));
		}

		LastLapTimeText = Cast<UTextBlock>(Widget->GetWidgetFromName(TEXT("LastLapTime")));
		if (LastLapTimeText)
		{
			FString String = FString("## YOUR LAST LAP TIME GOES HERE ##");
			LastLapTimeText->SetText(FText::FromString(String));
		}

		RacepaceTimingWidget->AddToViewport();
	}
}
