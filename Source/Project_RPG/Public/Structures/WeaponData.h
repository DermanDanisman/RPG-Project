// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponProperties.h"
#include "WeaponMontages.h"
#include "WeaponData.generated.h"

USTRUCT(BlueprintType)
struct FWeaponData : public FTableRowBase
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "Weapon Data")
	FWeaponProperties WeaponProperties;

	UPROPERTY(EditAnywhere, Category = "Weapon Data")
	FWeaponMontages WeaponStandardMontages;


	FWeaponData()
	{

	}
};
