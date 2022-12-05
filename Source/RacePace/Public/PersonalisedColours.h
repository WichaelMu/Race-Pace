// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PersonalisedColours.generated.h"

USTRUCT(BlueprintType)
struct FActivatableColourGroup
{
	GENERATED_BODY()


	UPROPERTY(EditAnywhere, Category = "Colour Personalisation")
		TArray<int32> ActivatedMaterialIndexes;

	UPROPERTY(EditAnywhere, meta = (UIMin = 0.0), Category = "Colour Personalisation")
		float EmissiveIntensity;
	float DefaultEmissiveIntensity;

	UPROPERTY(EditAnywhere, Category = "Colour Personalisation")
		FLinearColor ActivatedColour;
	FLinearColor DefaultActivatedColour;

	bool bIsDefaulted;

};

USTRUCT(BlueprintType)
struct FPersonalisedColourGroup
{
	GENERATED_BODY()


	UPROPERTY(EditAnywhere, Category = "Colour Personalisation")
		TArray<int32> PersonalisedMaterialIndexes;

	UPROPERTY(EditAnywhere, meta = (UIMin = 0.0), Category = "Colour Personalisation")
		float EmissiveIntensity;

	UPROPERTY(EditAnywhere, Category = "Colour Personalisation")
		FLinearColor PersonalisedColour;
};


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RACEPACE_API UPersonalisedColours : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPersonalisedColours();

protected:

	virtual void BeginPlay() override;


#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyThatChanged) override;
#endif

protected:

	UPROPERTY(EditAnywhere, Category = "Colour Personalisation", AdvancedDisplay)
		UMaterialInstance* ParentMaterial;

	UPROPERTY(EditAnywhere, Category = "Colour Personalisation")
		TArray<FPersonalisedColourGroup> PersonalisedColours;
	UPROPERTY(EditAnywhere, Category = "Colour Personalisation|Activatable")
		TArray<FActivatableColourGroup> ActivatableColours;

public:

	UFUNCTION(BlueprintCallable)
		void ChangeColourPredefined();
	UFUNCTION(BlueprintCallable)
		void ChangeColour(const TArray<FPersonalisedColourGroup>& ColourCollection);


	UFUNCTION(BlueprintCallable)
		void ActivateColour(const bool& bInActive, const int32& ActivatableIndex);


private:

	USkeletalMeshComponent* GetMesh() const;
	void SetMaterialProperties(USkeletalMeshComponent* Mesh, const int32& MaterialIndex, UMaterialInstanceDynamic* DynamicMaterial, const FLinearColor& Colour, const float& EmissiveIntensity);
	void AssignActivateColourDefaults();
};
