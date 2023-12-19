// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	EWT_None UMETA(DisplayName = "None"),
	EWT_OneHandedSword UMETA(DisplayName = "One-Handed Sword"),
	EWT_TwoHandedSword UMETA(DisplayName = "Two-Handed Sword"),
	EWT_SwordAndShield UMETA(DisplayName = "Sword and Shield")
};
