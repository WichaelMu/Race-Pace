// Fill out your copyright notice in the Description page of Project Settings.


#include "PersonalisedColours.h"
#include "RacePaceHelpers.h"

#include "Materials/MaterialInstanceDynamic.h"
#include "Racecar.h"


// Sets default values for this component's properties
UPersonalisedColours::UPersonalisedColours()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

	PersonalisedColour = FLinearColor(1.f, 0.494118f, 0.f);
	EmissiveIntensity = 1.f;
}


void UPersonalisedColours::PostEditChangeProperty(FPropertyChangedEvent& PropertyThatChanged)
{
	Super::PostEditChangeProperty(PropertyThatChanged);

	// ...

	FName PropertyName = PropertyThatChanged.GetPropertyName();
	if (PropertyName == FName("PersonalisedColour") || PropertyName == FName("EmissiveIntensity"))
	{
		ChangeColourPredefined();
	}
}


void UPersonalisedColours::ChangeColourPredefined()
{
	ChangeColour(PersonalisedColour, EmissiveIntensity);
}


void UPersonalisedColours::ChangeColour(const FLinearColor& InColour, const float& Intensity)
{
	if (AActor* Owner = GetOwner())
	{
		if (ARacecar* Racecar = Cast<ARacecar>(Owner))
		{
			if (USkeletalMeshComponent* Mesh = Racecar->GetMesh())
			{
				UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(ParentMaterial, NULL);

				for (int32 i = 0; i < PersonalisedMaterialIndexes.Num(); ++i)
				{
					int32 MaterialIndex = PersonalisedMaterialIndexes[i];
					if (UMaterialInterface* Material = Mesh->GetMaterial(MaterialIndex))
					{
						DynamicMaterial->SetVectorParameterValue(FName("StripeColour"), InColour);
						DynamicMaterial->SetScalarParameterValue(FName("Emissive Intensity"), EmissiveIntensity);
						Mesh->SetMaterial(MaterialIndex, DynamicMaterial);
					}
#if UE_BUILD_DEVELOPMENT
					else
					{
						EF("Invalid Material Index at: %i", MaterialIndex);
					}
#endif
				}
			}
		}
	}
}



