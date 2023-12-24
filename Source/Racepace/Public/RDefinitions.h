// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

// UE_LOG with a format.
#define LF(Format, ...) UE_LOG(LogTemp, Warning, TEXT("%i - %s"), __LINE__, *FString::Printf(TEXT(Format), __VA_ARGS__))
#define G(Format, ...) __FILE__, __FUNCTION__, __LINE__, *FString::Printf(TEXT(Format), __VA_ARGS__))
// UE_LOG a Message.
#define L(Message) UE_LOG(LogTemp, Warning, TEXT(Message))

// UE_LOG Error with a format.
#define EF(Format, ...) UE_LOG(LogTemp, Error, TEXT("%i - %s"), __LINE__, *FString::Printf(TEXT(Format), __VA_ARGS__))
// UE_LOG an Error Message.
#define E(Message) UE_LOG(LogTemp, Error, TEXT(Message))

// Pre-processor directive to disable a block of code.
#define DISABLE !UE_BUILD_DEVELOPMENT

// If Unreal is being PIE or SIE.
#define UNREAL_EDITOR UE_BUILD_DEVELOPMENT && !UE_BUILD_SHIPPING

// Mirrors Value about Minimum and Maximum. E.g., MIRROR_NUMBER(7, 0, 10) = 3.
#define MIRROR_NUMBER(Value, Minimum, Maximum) ((Minimum) + (Maximum)) - (Value)

#define CNULL(Pointer) if (!Pointer) { return; }
#define CNULL_RET(Pointer, RetVal) if (!Pointer) { return RetVal; }
#define CNULLP(Pointer) if (!Pointer) { EF("%s is null!", #Pointer) return; }
#define CNULLP_RET(Pointer, RetVal) if (!Pointer) { E(#Pointer) return RetVal; }

// Returns if Pointer is nullptr.
#define CNULLF(Pointer, Format, ...) if (!Pointer) { EF(Format, __VA_ARGS__) return; }
// Returns if Pointer is nullptr.
#define CNULL_M(Pointer, Message) if (!Pointer) { EM(Message) return; }

// Returns DefaultValue if Pointer is nullptr.
#define CNULL_RET_EF(Pointer, Message, DefaultValue) if (!Pointer) { EF(Message) return DefaultValue; }
// Returns DefaultValue if Pointer is nullptr.
#define CNULL_RET_M(Pointer, Format, DefaultValue, ...) if (!Pointer) { E(Format, __VA_ARGS__) return DefaultValue; }