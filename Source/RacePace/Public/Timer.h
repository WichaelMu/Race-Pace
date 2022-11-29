// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Containers/Set.h"

#include "Timer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBeginTimer, AActor*, InstigatingActor);

class ARacecar;

UCLASS()
class RACEPACE_API ATimer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATimer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintCallable)
		FBeginTimer OnEntered;

	UFUNCTION()
		void Enter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:

	TSet<ARacecar*> ActiveLaps;
};
