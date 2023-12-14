// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
/* Enums */
#include "Enums/LocomotionState.h"
#include "Enums/CharacterState.h"
/* Animation */
#include "Animation/CachedAnimData.h"
/* Interfaces*/
#include "Interfaces/PlayerInputInterface.h"
#include "Interfaces/ReferencesInterface.h"
#include "PlayerCharacterAnimInstance.generated.h"


/**
 * 
 */
UCLASS()
class PROJECT_RPG_API UPlayerCharacterAnimInstance : public UAnimInstance, public IReferencesInterface
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;
	virtual void NativePostEvaluateAnimation() override;

	UFUNCTION(BlueprintCallable)
	void SetReferences();

	UFUNCTION(BlueprintCallable)
	void SetCachedAnimStateData();

	UFUNCTION(BlueprintCallable)
	void SetEssentialMovementData();

	UFUNCTION(BlueprintCallable)
	void DetermineLocomotionState();

	UFUNCTION(BlueprintCallable)
	bool IsMovementWithinThresholds(float MinCurrentSpeed, float MinMaxSpeed, float MinInputAcceleration);

	UFUNCTION(BlueprintCallable)
	void UpdateLocomotionValues(FName CurveName);

	UFUNCTION(BlueprintCallable)
	void UpdateOnWalkEntry();

	UFUNCTION(BlueprintCallable)
	void UpdateOnJogEntry();

	UFUNCTION(BlueprintCallable)
	void UpdateOnSprintEntry();

	UFUNCTION(BlueprintCallable)
	void ResetTargetRotations();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FRotator GetTargetRotation();

	UFUNCTION(BlueprintCallable)
	void UpdateCharacterRotation();

	UFUNCTION(BlueprintCallable)
	void UpdateRotationForState(const FCachedAnimStateData& StateData, const FName& CurveName);

	UFUNCTION(BlueprintCallable)
	void ResetTransitions();

	UFUNCTION()
	void AimOffsetCalculation();

public:

	/* References Category */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "References")
	class APlayerCharacter* PlayerCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "References")
	class UCharacterMovementComponent* CharacterMovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "References")
	UAnimInstance* AnimInstance;

	/* Essential Data Category */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Essential Data")
	float DeltaTimeX;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Essential Data")
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Essential Data")
	FVector InputVector;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Essential Data")
	float GroundSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Essential Data")
	float Direction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Essential Data")
	float MaxSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Essential Data")
	float CharacterYawRotationRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Essential Data")
	bool bIsFalling;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Essential Data")
	bool bOrientRotationToMovement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Essential Data")
	bool bUseControllerDesiredRotation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Essential Data")
	ECharacterState CharacterState;

	/* Locomotion Category */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Locomotion")
	ELocomotionState LocomotionState;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Locomotion")
	float PlayRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Locomotion")
	float WalkStartAngle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Locomotion")
	float JogStartAngle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Locomotion")
	float SprintStartAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Locomotion")
	bool bPlayWalkStart;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Locomotion")
	bool bPlayJogStart;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Locomotion")
	bool bPlaySprintStart;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Locomotion")
	bool bWalkOnEntryFlag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Locomotion")
	bool bJogOnEntryFlag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Locomotion")
	bool bSprintOnEntryFlag;

	/* Rotation Category */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rotation")
	bool bDoInputVectorRotation = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation")
	FRotator StartRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation")
	FRotator PrimaryTargetRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation")
	FRotator SecondaryTargetRotation;

	/* State Data Category*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State Data")
	FCachedAnimStateData LocomotionStateData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State Data")
	FCachedAnimStateData WalkStateData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State Data")
	FCachedAnimStateData JogStateData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State Data")
	FCachedAnimStateData SprintStateData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AimOffset Data")
	float AimOffsetYaw;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AimOffset Data")
	float AimOffsetPitch;

private:

	UFUNCTION()
	virtual UPlayerCharacterAnimInstance* RI_GetPlayerAnimInstance_Implementation() const override;
};
