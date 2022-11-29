// Fill out your copyright notice in the Description page of Project Settings.


#include "Dashboard.h"
#include "RacePaceHelpers.h"

#include "UObject/ConstructorHelpers.h"
#include "Components/TextBlock.h"


UDashboard::UDashboard()
{
	SetWidgetSpace(EWidgetSpace::World);

	static ConstructorHelpers::FClassFinder<UUserWidget> UIDash(TEXT("/Game/Widgets/UI_Dashboard"));
	if (UIDash.Succeeded())
	{
		SetWidgetClass(UIDash.Class);
	}

	SetRelativeRotation(FRotator(0.f, 180.f, 0.f));
}

void UDashboard::BeginPlay()
{
	Super::BeginPlay();

	// ...

	if (UUserWidget* UserWidget = CreateWidget<UUserWidget>(GetWorld(), GetWidgetClass()))
	{
		if (UTextBlock* RPMText = Cast<UTextBlock>(UserWidget->GetWidgetFromName("RPM")))
		{
			RPM = RPMText;
		}

		if (UTextBlock* SpeedometerText = Cast<UTextBlock>(UserWidget->GetWidgetFromName("Speedometer")))
		{
			Speedometer = SpeedometerText;
		}

		if (UTextBlock* GearText = Cast<UTextBlock>(UserWidget->GetWidgetFromName("Gear")))
		{
			Gear = GearText;
		}
	}

	// Null checks.
	CNULLP(Speedometer)
	CNULLP(RPM)
	CNULLP(Gear)
}