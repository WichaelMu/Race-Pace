// Fill out your copyright notice in the Description page of Project Settings.


#include "PersonalisedColours.h"
#include "RDefinitions.h"

#include "Materials/MaterialInterface.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Racecar.h"


// Sets default values for this component's properties
UPersonalisedColours::UPersonalisedColours()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

	PersonalisedColours.SetNum(1);
	ActivatableColours.SetNum(0);
}

#if WITH_EDITOR
void UPersonalisedColours::BeginPlay()
{
	Super::BeginPlay();

	// ...

	ChangeColourPredefined();
	AssignActivateColourDefaults();
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
#endif


void UPersonalisedColours::ChangeColourPredefined()
{
	ChangeColour(PersonalisedColours);
}


void UPersonalisedColours::ChangeColour(const TArray<FPersonalisedColourGroup>& ColourCollection)
{
	if (USkeletalMeshComponent* Mesh = GetMesh())
	{
		for (int32 ColourGroup = 0; ColourGroup < ColourCollection.Num(); ++ColourGroup)
		{
			UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(ParentMaterial, nullptr);

			for (int32 i = 0; i < ColourCollection[ColourGroup].PersonalisedMaterialIndexes.Num(); ++i)
			{
				const FPersonalisedColourGroup& Group = ColourCollection[ColourGroup];
				const int32& MaterialIndex = Group.PersonalisedMaterialIndexes[i];

				SetMaterialProperties(Mesh, MaterialIndex, DynamicMaterial, Group.PersonalisedColour, Group.EmissiveIntensity);
			}
		}
	}
}

void UPersonalisedColours::ActivateColour(const bool& bInActive, const int32& ActivatableIndex)
{
	if (ActivatableColours.Num() == 0)
	{
		return;
	}

	if (!ActivatableColours.IsValidIndex(ActivatableIndex))
	{
		EF("Invalid Activatable Index. Expected 0 <= %i <= %i", ActivatableIndex, ActivatableColours.Num());
		return;
	}

	// Don't bother activating/deactivating when it's already been activated/deactivated.
	FActivatableColourGroup& Group = ActivatableColours[ActivatableIndex];
	if (Group.bIsDefaulted == bInActive)
	{
		return;
	}

	Group.bIsDefaulted = bInActive;

	if (USkeletalMeshComponent* Mesh = GetMesh())
	{
		UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(ParentMaterial, nullptr);

		for (int32 i = 0; i < Group.ActivatedMaterialIndexes.Num(); ++i)
		{
			if (bInActive)
			{
				SetMaterialProperties(Mesh, Group.ActivatedMaterialIndexes[i], DynamicMaterial, Group.ActivatedColour, Group.EmissiveIntensity);
			}
			else
			{
				SetMaterialProperties(Mesh, Group.ActivatedMaterialIndexes[i], DynamicMaterial, Group.DefaultActivatedColour, Group.DefaultEmissiveIntensity);
			}
		}
	}
}


void UPersonalisedColours::SetMaterialProperties(USkeletalMeshComponent* Mesh, const int32& MaterialIndex, UMaterialInstanceDynamic* DynamicMaterial, const FLinearColor& Colour, const float& EmissiveIntensity)
{
	if (UMaterialInterface* Material = Mesh->GetMaterial(MaterialIndex))
	{
		DynamicMaterial->SetVectorParameterValue(FName("Colour"), Colour);
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


void UPersonalisedColours::AssignActivateColourDefaults()
{
	if (USkeletalMeshComponent* Mesh = GetMesh())
	{
		for (FActivatableColourGroup& Group : ActivatableColours)
		{
			if (Group.ActivatedMaterialIndexes.Num() > 0)
			{
				if (UMaterialInterface* Material = Mesh->GetMaterial(Group.ActivatedMaterialIndexes[0]))
				{
					FHashedMaterialParameterInfo ColourParameter = FHashedMaterialParameterInfo("Colour");
					FHashedMaterialParameterInfo EmissiveIntensityParameter = FHashedMaterialParameterInfo("Emissive Intensity");

					FLinearColor DefaultColour;
					if (Material->GetVectorParameterValue(ColourParameter, DefaultColour))
					{
						Group.DefaultActivatedColour = DefaultColour;
					}

					float DefaultEmissiveIntensity;
					if (Material->GetScalarParameterValue(EmissiveIntensityParameter, DefaultEmissiveIntensity))
					{
						Group.DefaultEmissiveIntensity = DefaultEmissiveIntensity;
					}
				}
			}
		}
	}
}


USkeletalMeshComponent* UPersonalisedColours::GetMesh() const
{
	if (AActor* Owner = GetOwner())
	{
		if (ARacecar* Racecar = Cast<ARacecar>(Owner))
		{
			if (USkeletalMeshComponent* Mesh = Racecar->GetMesh())
			{
				return Mesh;
			}
		}
	}

	return nullptr;
}
