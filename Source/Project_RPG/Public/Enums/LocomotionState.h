// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

UENUM(BlueprintType)
enum class ELocomotionState : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Walking UMETA(DisplayName = "Walking"),
	Jogging UMETA(DisplayName = "Jogging"),
	Sprinting UMETA(DisplayName = "Sprinting"),

	EW_MAX UMETA(DisplayName = "DefaultMAX")
};
