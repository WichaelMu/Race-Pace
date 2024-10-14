// Racepace is a game built by WichaelMu.


#include "SyntheticSound.h"

#include "RDefinitions.h"


USyntheticSound::USyntheticSound(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// ...

	bLooping = true;
	NumChannels = 1;
	SampleRate = 44100;

	EnginePhase = .0f;
	RacecarRPM = 1000.0f;
	RacecarThrottle = .0f;
	EngineTurboFactor = .0f;
	EngineFrequency = .0f;
	CurrentEngineType = EEngineType::ICEV10High;
	bRacecarIsTurbo = false;
}

void USyntheticSound::SetEngineType(EEngineType EngineType, bool bIsTurbocharged)
{
	CurrentEngineType = EngineType;
	bRacecarIsTurbo = bIsTurbocharged;
}

void USyntheticSound::SetRPM(float InRPM)
{
	RacecarRPM = InRPM;

	// Frequency = RacecarRPM / 60.0f * 0.5f;
	EngineFrequency = RacecarRPM * 0.333333F;
}

void USyntheticSound::SetThrottle(float ThrottleAmount)
{
	RacecarThrottle = FMath::Clamp(ThrottleAmount, 0.0f, 1.0f);
	EngineTurboFactor = bRacecarIsTurbo ? ThrottleAmount : 0.0f;
}

void USyntheticSound::GenerateEngineSound(float& OutSample, float Frequency, float ThrottleAmount)
{
	switch (CurrentEngineType)
	{
	case EEngineType::ICEV10Low:
	case EEngineType::ICEV10High:

		OutSample = FMath::Sin(EnginePhase * 2.0f * PI) * 0.5f + FMath::Sin(EnginePhase * 6.0f * PI) * 0.3f;
		break;

	case EEngineType::ICEV8Low:
	case EEngineType::ICEV8High:
		OutSample = FMath::Sin(EnginePhase * 2.0f * PI) * 0.6f + FMath::Sin(EnginePhase * 4.0f * PI) * 0.4f;
		break;

	case EEngineType::Electric:
		OutSample = FMath::Sin(EnginePhase * 8.0f * PI) * 0.2f + FMath::Sin(EnginePhase * 16.0f * PI) * 0.1f;
		break;

	default:
		OutSample = FMath::Sin(EnginePhase * 2.0f * PI) * 0.4f;  // Default
		break;
	}

	// Scale by throttle
	OutSample *= ThrottleAmount;
}

void USyntheticSound::AddTurboWhine(float& OutSample, float TurboFactor)
{
	// Add a high-frequency turbo whine based on throttle
	float TurboWhine = FMath::Sin(EnginePhase * 12.0f * PI) * TurboFactor * 0.3f;
	OutSample += TurboWhine;
}

int32 USyntheticSound::GeneratePCMData(uint8* PCMData, const int32 SamplesNeeded)
{
	int32 SamplesGenerated = 0;

	while (SamplesGenerated < SamplesNeeded)
	{
		float SampleValue = 0.0f;

		// Generate base engine sound
		GenerateEngineSound(SampleValue, EngineFrequency, RacecarThrottle);

		// Add turbo sound if turbocharged
		if (bRacecarIsTurbo)
		{
			AddTurboWhine(SampleValue, EngineTurboFactor);
		}

		// Convert float (-1.0 to 1.0) to int16 PCM format
		int16 PCMValue = (int16)(SampleValue * 32767.0f);

		// Copy to the PCMData buffer
		PCMData[SamplesGenerated++] = PCMValue & 0xFF;
		PCMData[SamplesGenerated++] = (PCMValue >> 8) & 0xFF;

		// Update phase
		EnginePhase += EngineFrequency / SampleRate;
		if (EnginePhase > 1.0f)
		{
			EnginePhase -= 1.0f;
		}
	}

	return SamplesGenerated;
}
