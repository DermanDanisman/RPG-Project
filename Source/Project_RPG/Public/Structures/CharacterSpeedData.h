// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterSpeedData.generated.h"

USTRUCT(BlueprintType)
struct FCharacterSpeedData : public FTableRowBase
{
    GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "Movement Essential Data")
	float JumpVelocity;

	UPROPERTY(EditAnywhere, Category = "Movement Essential Data")
	float WalkingSpeed;

	UPROPERTY(EditAnywhere, Category = "Movement Essential Data")
	float JoggingSpeed;

	UPROPERTY(EditAnywhere, Category = "Movement Essential Data")
	float SprintingSpeed;

	UPROPERTY(EditAnywhere, Category = "Movement Essential Data")
	float CrouchingSpeed;

    FCharacterSpeedData()
    {
		JumpVelocity = 520.f;
		WalkingSpeed = 147.f;
		JoggingSpeed = 427.f;
		SprintingSpeed = 627.f;
		CrouchingSpeed = 247.f;
    }
};

