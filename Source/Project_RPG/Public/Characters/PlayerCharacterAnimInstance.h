// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
/* Enums */
#include "Enums/LocomotionState.h"
/* Animation */
#include "Animation/CachedAnimData.h"
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
	virtual void NativePostEvaluateAnimation() override;

	UFUNCTION()
	void SetReferences();

	UFUNCTION()
	void SetCachedAnimStateData();

	UFUNCTION()
	void SetEssentialMovementData();

	UFUNCTION()
	void DetermineLocomotionState();

	UFUNCTION()
	bool IsMovementWithinThresholds(float MinCurrentSpeed, float MinMaxSpeed, float MinInputAcceleration);

	UFUNCTION()
	void UpdateLocomotionValues(FName CurveName);

	UFUNCTION()
	void UpdateOnWalkEntry();

	UFUNCTION()
	void UpdateOnJogEntry();

	UFUNCTION()
	void ResetTargetRotations();

	UFUNCTION()
	FRotator GetTargetRotation();

	UFUNCTION()
	void UpdateCharacterRotation();

	UFUNCTION()
	void ResetTransitions();

public:

	UPROPERTY(BlueprintReadOnly, Category = "References")
	class APlayerCharacter* PlayerCharacter;

	UPROPERTY(BlueprintReadOnly, Category = "References")
	class UCharacterMovementComponent* CharacterMovementComponent;

	UPROPERTY(BlueprintReadOnly, Category = "References")
	UAnimInstance* AnimInstance;

	UPROPERTY(BlueprintReadOnly, Category = "Essential Data")
	float DeltaTimeX;
	
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

	UPROPERTY(BlueprintReadOnly, Category = "Locomotion")
	float WalkStartAngle;

	UPROPERTY(BlueprintReadOnly, Category = "Locomotion")
	float JogStartAngle;

	UPROPERTY(BlueprintReadOnly, Category = "Rotation")
	bool bDoInputVectorRotation = false;

	UPROPERTY(BlueprintReadOnly, Category = "Rotation")
	FRotator StartRotation;

	UPROPERTY(BlueprintReadOnly, Category = "Rotation")
	FRotator PrimaryTargetRotation;

	UPROPERTY(BlueprintReadOnly, Category = "Rotation")
	FRotator SecondaryTargetRotation;

	UPROPERTY(BlueprintReadOnly, Category = "Rotation")
	bool bPlayWalkStart;

	UPROPERTY(BlueprintReadOnly, Category = "Rotation")
	bool bPlayJogStart;

	UPROPERTY(BlueprintReadOnly, Category = "State Data")
	FCachedAnimStateData LocomotionStateData;

	UPROPERTY(BlueprintReadOnly, Category = "State Data")
	FCachedAnimStateData WalkStateData;

	UPROPERTY(BlueprintReadOnly, Category = "State Data")
	FCachedAnimStateData JogStateData;

	UPROPERTY(BlueprintReadOnly, Category = "State Data")
	FCachedAnimStateData SprintStateData;



};
