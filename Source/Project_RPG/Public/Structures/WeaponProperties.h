// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enums/WeaponType.h"
#include "WeaponProperties.generated.h"

USTRUCT(BlueprintType)
struct FWeaponProperties
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "Weapon Essential Data")
	EWeaponType WeaponType;

	UPROPERTY(EditAnywhere, Category = "Weapon Essential Data")
	FName OneHandedHandSocket;

	UPROPERTY(EditAnywhere, Category = "Weapon Essential Data")
	FName TwoHandedHandSocket;

	UPROPERTY(EditAnywhere, Category = "Weapon Essential Data")
	FName HipWeaponHolsterSocket;

	UPROPERTY(EditAnywhere, Category = "Weapon Essential Data")
	FName BackWeaponHolsterSocket;

	FWeaponProperties()
	{
		WeaponType = EWeaponType::EWT_None;
		OneHandedHandSocket = FName("One-HandedWeaponSocket");
		TwoHandedHandSocket = FName("Two-HandedWeaponSocket");
		HipWeaponHolsterSocket = FName("HipWeaponHolsterSocket");
		BackWeaponHolsterSocket = FName("BackWeaponHolsterLocation");
	}
};
