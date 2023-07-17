// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RacecarUI.h"

#include "Components/WidgetComponent.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

#include "RacecarUIController.generated.h"


#define ADD_HUD_ELEMENT(VarName) \
UPROPERTY(BlueprintReadWrite) \
UTextBlock* VarName;\

class ARacepacePlayer;
class ARacecar;

class UCanvasPanelSlot;


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RACEPACE_API URacecarUIController : public URacecarUI
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	URacecarUIController();

protected:

	// Called when the game starts
	virtual void BeginPlay() override;


public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


public:

	// Lap Time Functions.
	void SetLapTime(const FString& InTime);
	void SetBestLapTime(const FString& InBestTime);
	void SetLastLapTime(const FString& InLastLapTime);
	FORCEINLINE void SetLapTime(const float InTime);
	FORCEINLINE void SetBestLapTime(const float InBestTime);
	FORCEINLINE void SetLastLapTime(const float InLastLapTime);
	void CompareLapToBestDeltas(const float& LapTime, const float& BestTime);

	// Dashboard Functions.
	void SetSpeed(const int32 InKMPH);
	void SetRPM(const int32 InRPM);
	void SetGear(const FString InGear);

public:


	TSubclassOf<UUserWidget> DashboardHUDWidget;

	// Lap Time UI Elements.
	ADD_HUD_ELEMENT(TimingText);
	ADD_HUD_ELEMENT(BestTimeText);
	ADD_HUD_ELEMENT(LapDeltaText);
	ADD_HUD_ELEMENT(LastLapTimeText);

	// Dashboard UI Elements.
	ADD_HUD_ELEMENT(KMPHSpeedText);
	ADD_HUD_ELEMENT(GearText);
	ADD_HUD_ELEMENT(RPMText);
	ADD_HUD_ELEMENT(ShiftIndicator);

	UImage* RPMCurrent;
	UCanvasPanelSlot* RPMCurrentSlot;
	UCanvasPanelSlot* RPMTextSlot;
	TArray<UImage*> RPMBlocks;

	const FSlateColor NegativeDeltaColour = FSlateColor(FLinearColor(FColor(255.f, 68.f, 68.f)));
	const FSlateColor PositiveDeltaGreen = FSlateColor(FLinearColor(FColor(68.f, 255.f, 68.f)));


protected:

	UUserWidget* RacepacePlayerWidget;

public:

	ARacecar* GetRacecar() const;
	UUserWidget* GetRacepaceWidget() const;

private:

	void CalculateRPMGraphics(const int32 RPM, const float DeltaTime);

	static float CurveFunction(const float& Ratio, const float& Scalar);


private:

	UPROPERTY(EditDefaultsOnly)
		FString ShiftUpText;
	float RevUpRPM;
	bool bShouldShowDownShiftIndicator;

};

#undef ADD_HUD_ELEMENT
