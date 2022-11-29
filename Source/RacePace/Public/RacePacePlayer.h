// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Components/WidgetComponent.h"
#include "Components/TextBlock.h"

#include "RacePacePlayer.generated.h"

#define ADD_HUD_ELEMENT(VarName) \
UPROPERTY(BlueprintReadWrite) \
UTextBlock* VarName;\

class ULapTimer;

/**
 * 
 */
UCLASS()
class RACEPACE_API ARacePacePlayer : public APlayerController
{
	GENERATED_BODY()

public:

	ARacePacePlayer();

	void SetLapTime(const FString& InTime);
	void SetBestLapTime(const FString& InBestTime);
	void SetLastLapTime(const FString& InLastLapTime);
	void CompareLapToBestDeltas(const float& LapTime, const float& BestTime, const ULapTimer* LapTimingComponent);


protected:

	virtual void BeginPlay() override;


public:


	TSubclassOf<UUserWidget> TimingWidget;
	UUserWidget* RacepaceTimingWidget;

	ADD_HUD_ELEMENT(TimingText)
	ADD_HUD_ELEMENT(BestTimeText)
	ADD_HUD_ELEMENT(LapDeltaText)
	ADD_HUD_ELEMENT(LastLapTimeText)

	const FSlateColor NegativeDeltaColour = FSlateColor(FLinearColor(FColor(255.f, 68.f, 68.f)));
	const FSlateColor PositiveDeltaGreen = FSlateColor(FLinearColor(FColor(68.f, 255.f, 68.f)));
	
};
