// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	ECS_Unequipped UMETA(DisplayName = "Unequipped"),
	ECS_OneHandedSword UMETA(DisplayName = "One Handed Sword"),
	ECS_TwoHandedSword UMETA(DisplayName = "Two Handed Sword"),
	ECS_SwordAndShield UMETA(DisplayName = "Sword And Shield"),

	ECS__MAX UMETA(DisplayName = "DefaultMAX")
};
