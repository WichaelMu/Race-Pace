// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PersonalisedColours.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RACEPACE_API UPersonalisedColours : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPersonalisedColours();

protected:

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyThatChanged);

protected:

	UPROPERTY(EditAnywhere, Category="Colour Personalisation")
		UMaterialInstance* ParentMaterial;

	UPROPERTY(EditAnywhere, Category="Colour Personalisation")
		TArray<int32> PersonalisedMaterialIndexes;

	UPROPERTY(EditAnywhere, Category="Colour Personalisation")
		FLinearColor PersonalisedColour;
	UPROPERTY(EditAnywhere, meta = (UIMin = 0.0), Category = "Colour Personalisation")
		float EmissiveIntensity;

public:

	UFUNCTION(BlueprintCallable)
		void ChangeColourPredefined();
	UFUNCTION(BlueprintCallable)
		void ChangeColour(const FLinearColor& InColour, const float& Intensity);

};
