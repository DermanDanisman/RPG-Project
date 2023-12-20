// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enums/WeaponType.h"
#include "WeaponProperties.generated.h"

USTRUCT(BlueprintType)
struct FWeaponProperties : public FTableRowBase
{

	GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Essential Data")
    EWeaponType WeaponType;

    // Reference to the Static Mesh asset
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Essential Data")
    UStaticMesh* WeaponMesh;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Essential Data")
    FName WeaponSocket;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Essential Data")
    FName HipWeaponHolsterSocket;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Essential Data")
    FName BackWeaponHolsterSocket;

    FWeaponProperties()
    {
        WeaponType = EWeaponType::EWT_None;
        WeaponMesh = nullptr;
        WeaponSocket = FName("One-HandedWeaponSocket");
        HipWeaponHolsterSocket = FName("HipWeaponHolsterSocket");
        BackWeaponHolsterSocket = FName("BackWeaponHolsterLocation");
    }
};
