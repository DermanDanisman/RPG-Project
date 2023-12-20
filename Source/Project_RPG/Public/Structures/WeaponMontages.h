// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponMontages.generated.h"

USTRUCT(BlueprintType)
struct FWeaponMontages : public FTableRowBase
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "Weapon Standart Montages")
	UAnimMontage* DrawWeaponMontage;

	UPROPERTY(EditAnywhere, Category = "Weapon Standart Montages")
	UAnimMontage* HolsterWeaponMontage;

	UPROPERTY(EditAnywhere, Category = "Weapon Standart Montages")
	UAnimMontage* DodgeMontage;


	FWeaponMontages()
	{
		DrawWeaponMontage = nullptr;
		HolsterWeaponMontage = nullptr;
		DodgeMontage = nullptr;
	}
};
