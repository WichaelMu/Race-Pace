// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RacetrackEvent.generated.h"

class ARacecar;
class UBoxComponent;

#define OVERRIDE_ENTER_FUNCTION() UFUNCTION(BlueprintCallable) \
	virtual void Enter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override; \

#define IMPLEMENT_ENTER_FUNCTION(ClassName) void ClassName::Enter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)

UCLASS()
class RACEPACE_API ARacetrackEvent : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARacetrackEvent();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:

	UFUNCTION(BlueprintCallable)
		virtual void Enter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	FORCEINLINE ARacecar* CastToRacecar(AActor* OtherActor);

protected:

	UPROPERTY(EditAnywhere)
		UBoxComponent* Collider;
};
