// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
/* Enums */
#include "Enums/LocomotionState.h"
#include "PlayerCharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_RPG_API UPlayerCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	UFUNCTION()
	void SetReferences();

	UFUNCTION()
	void SetEssentialMovementData();

	UFUNCTION()
	void DetermineLocomotionState();

	UFUNCTION()
	bool IsMovementWithinThresholds(float MinCurrentSpeed, float MinMaxSpeed, float MinInputAcceleration);

	UFUNCTION()
	void UpdateLocomotionValues(FName CurveName);
public:

	UPROPERTY(BlueprintReadOnly, Category = "References")
	class APlayerCharacter* PlayerCharacter;

	UPROPERTY(BlueprintReadOnly, Category = "References")
	class UCharacterMovementComponent* CharacterMovementComponent;
	
	UPROPERTY(BlueprintReadOnly, Category = "Essential Data")
	FVector Velocity;

	UPROPERTY(BlueprintReadOnly, Category = "Essential Data")
	float GroundSpeed;

	UPROPERTY(BlueprintReadOnly, Category = "Essential Data")
	float Direction;

	UPROPERTY(BlueprintReadOnly, Category = "Essential Data")
	float MaxSpeed;

	UPROPERTY(BlueprintReadOnly, Category = "Essential Data")
	FVector InputVector;

	UPROPERTY(BlueprintReadOnly, Category = "Essential Data")
	bool bIsFalling;

	UPROPERTY(BlueprintReadOnly, Category = "Essential Data")
	bool bOrientRotationToMovement;

	UPROPERTY(BlueprintReadOnly, Category = "Locomotion")
	ELocomotionState LocomotionState;

	UPROPERTY(BlueprintReadOnly, Category = "Locomotion")
	float PlayRate;
};
