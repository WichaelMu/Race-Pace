// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "Dashboard.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class RACEPACE_API UDashboard : public UWidgetComponent
{
	GENERATED_BODY()

public:

	UDashboard();

	virtual void BeginPlay() override;

public:

	UPROPERTY(BlueprintReadWrite)
		UTextBlock* RPM;
	UPROPERTY(BlueprintReadWrite)
		UTextBlock* Speedometer;
	UPROPERTY(BlueprintReadWrite)
		UTextBlock* Gear;
};
