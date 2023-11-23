// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PlayerCharacterAnimInstance.h"
#include "Characters/PlayerCharacter.h"
/* Game Framework*/
#include "GameFramework/CharacterMovementComponent.h"
/* Kismet */
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

void UPlayerCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	SetReferences();
}

void UPlayerCharacterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (PlayerCharacter)
	{
		SetEssentialMovementData();
		DetermineLocomotionState();
		UpdateLocomotionValues(FName("MoveData_Speed"));
	}
}

void UPlayerCharacterAnimInstance::SetReferences()
{
	PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());
	if (PlayerCharacter)
	{
		CharacterMovementComponent = Cast<UCharacterMovementComponent>(PlayerCharacter->GetCharacterMovement());
	}
}

/* This function updates various movement-related properties: Velocity, 
GroundSpeed (the magnitude of the velocity on the XY plane), 
MaxSpeed (the character's maximum speed), 
InputVector (last input vector, clamped between 0 and 1), 
and bIsFalling (whether the character is falling).*/
void UPlayerCharacterAnimInstance::SetEssentialMovementData()
{
	if (CharacterMovementComponent)
	{
		Velocity = CharacterMovementComponent->Velocity;
		// Getting Magnitude of Velocity vector in XY axis
		//GroundSpeed = Velocity.Size();
		GroundSpeed = UKismetMathLibrary::VSizeXY(Velocity);
		Direction = CalculateDirection(Velocity, PlayerCharacter->GetActorRotation());
		//Direction = CalculateDirection(Velocity, PlayerCharacter->GetControlRotation());
		MaxSpeed = CharacterMovementComponent->GetMaxSpeed();
		InputVector = UKismetMathLibrary::ClampVectorSize(CharacterMovementComponent->GetLastInputVector(), 0.f, 1.f);
		bIsFalling = CharacterMovementComponent->IsFalling();
		bOrientRotationToMovement = CharacterMovementComponent->bOrientRotationToMovement;
	}
}

/* This function determines the character's current locomotion state (like idle, walking, jogging, sprinting) based on their movement data.
It uses the velocity, acceleration, and thresholds (determined by IsMovementWithinThresholds) to set LocomotionState.*/
void UPlayerCharacterAnimInstance::DetermineLocomotionState()
{
	if (CharacterMovementComponent)
	{
		if (!bIsFalling)
		{
			FVector NormalizedVelocity = Velocity.GetSafeNormal();
			FVector NormalizedCurrentAcceleration = CharacterMovementComponent->GetCurrentAcceleration().GetSafeNormal();
			if (UKismetMathLibrary::Dot_VectorVector(NormalizedVelocity, NormalizedCurrentAcceleration) < -0.5f)
			{
				LocomotionState = ELocomotionState::Idle;
			}
			else
			{
				if (IsMovementWithinThresholds(350.f, 500.f, 0.5f)) LocomotionState = ELocomotionState::Sprinting;
				else if (IsMovementWithinThresholds(1.0f, 300.f, 0.5f)) LocomotionState = ELocomotionState::Jogging;
				else if (IsMovementWithinThresholds(1.0f, 0.0f, 0.01f)) LocomotionState = ELocomotionState::Walking;
				else LocomotionState = ELocomotionState::Idle;
			}
		}
	}
}

/*  A utility function that checks if the current movement is within certain thresholds. 
It compares GroundSpeed, MaxSpeed, and the size of the InputVector against provided minimum values and returns true if all conditions are met.*/
bool UPlayerCharacterAnimInstance::IsMovementWithinThresholds(float MinCurrentSpeed, float MinMaxSpeed, float MinInputAcceleration)
{
	if (MinCurrentSpeed <= GroundSpeed && MinMaxSpeed <= MaxSpeed && MinInputAcceleration <= InputVector.Size()) return true;
	else return false;
}

/* This function updates animation-related values based on locomotion.
It gets a curve value (using GetCurveValue) identified by CurveName, clamps this value between 50 and 1000, and then calculates PlayRate by dividing GroundSpeed by the clamped curve value.
PlayRate likely controls the speed of the character's animations.*/
void UPlayerCharacterAnimInstance::UpdateLocomotionValues(FName CurveName)
{
	float ClampedCurveValues = UKismetMathLibrary::Clamp(GetCurveValue(CurveName), 50.f, 1000.f);
	PlayRate = UKismetMathLibrary::SafeDivide(GroundSpeed, ClampedCurveValues);
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, FString::Printf(TEXT("PlayRate: %f"), PlayRate));

}
