// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IRacecarUI.h"


#include "DashboardHUD.generated.h"

class ARacePacePlayer;
class ARacecar;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RACEPACE_API UDashboardHUD : public UIRacecarUI
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDashboardHUD();


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

};
