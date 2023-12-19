// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

UENUM(BlueprintType)
enum class EComboInputs : uint8
{
	ECI_None UMETA(DisplayName = "None"),
	ECI_LightAttack UMETA(DisplayName = "Light Attack"),
	ECI_HeavyAttack UMETA(DisplayName = "Heavy Attack"),
	ECI_InputX UMETA(DisplayName = "Input X"),
	ECI_InputY UMETA(DisplayName = "Input Y"),
	ECI_InputZ UMETA(DisplayName = "Input Z"),
	ECI_InputW UMETA(DisplayName = "Input W"),

	ECI_MAX UMETA(DisplayName = "DefaultMAX")
};
