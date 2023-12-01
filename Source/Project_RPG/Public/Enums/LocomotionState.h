// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

UENUM(BlueprintType)
enum class ELocomotionState : uint8
{
	ELS_Idle UMETA(DisplayName = "Idle"),
	ELS_Walking UMETA(DisplayName = "Walking"),
	ELS_Jogging UMETA(DisplayName = "Jogging"),
	ELS_Sprinting UMETA(DisplayName = "Sprinting"),

	ELS_MAX UMETA(DisplayName = "DefaultMAX")
};
