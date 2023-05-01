// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RacecarUI.generated.h"

class ARacecar;
class URacecarUIController;
class ARacepacePlayer;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RACEPACE_API URacecarUI : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URacecarUI();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
public:

	UPROPERTY(VisibleAnywhere, Category = "Racecar UI")
		ARacecar* Racecar;
	UPROPERTY(VisibleAnywhere, Category = "Racecar UI")
		URacecarUIController* RacecarUIController;

	FORCEINLINE void SetPlayer(ARacepacePlayer* InPlayer);
	ARacepacePlayer* GetPlayer();


private:

	ARacepacePlayer* Player;
};
