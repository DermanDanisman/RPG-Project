// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PlayerCharacterAnimInstance.h"
#include "Characters/PlayerCharacter.h"
/* Game Framework*/
#include "GameFramework/CharacterMovementComponent.h"
/* Kismet */
#include "Kismet/KismetMathLibrary.h"

void UPlayerCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	SetReferences();
}

void UPlayerCharacterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (PlayerCharacter && CharacterMovementComponent)
	{
		SetEssentialMovementData();
		DetermineLocomotionState();
	}
	//FString String = Velocity.ToString();
	//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, *FString::Printf(TEXT("Velocity: %s"), *String));
}

void UPlayerCharacterAnimInstance::SetReferences()
{
	PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());
	if (PlayerCharacter)
	{
		CharacterMovementComponent = Cast<UCharacterMovementComponent>(PlayerCharacter->GetCharacterMovement());
	}
}

void UPlayerCharacterAnimInstance::SetEssentialMovementData()
{
	if (CharacterMovementComponent)
	{
		Velocity = CharacterMovementComponent->Velocity;
		// Getting Magnitude of Velocity vector in XY axis
		GroundSpeed = Velocity.Size();
		//GroundSpeed = UKismetMathLibrary::VSizeXY(Velocity);
		MaxSpeed = CharacterMovementComponent->GetMaxSpeed();
		InputVector = UKismetMathLibrary::ClampVectorSize(CharacterMovementComponent->GetLastInputVector(), 0.f, 1.f);
		bIsFalling = CharacterMovementComponent->IsFalling();
	}
}

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

bool UPlayerCharacterAnimInstance::IsMovementWithinThresholds(float MinCurrentSpeed, float MinMaxSpeed, float MinInputAcceleration)
{
	/*GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, *FString::Printf(TEXT("Min Current Speed: %f"), MinCurrentSpeed));
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, *FString::Printf(TEXT("Ground Speed: %f"), GroundSpeed));

	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, *FString::Printf(TEXT("Min Max Speed: %f"), MinMaxSpeed));
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, *FString::Printf(TEXT("Max Speed: %f"), MaxSpeed));

	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, *FString::Printf(TEXT("Input Vector Size: %f"), InputVector.Size()));
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, *FString::Printf(TEXT("Min Input Acceleration: %f"), MinInputAcceleration));*/

	if (MinCurrentSpeed <= GroundSpeed && MinMaxSpeed <= MaxSpeed && MinInputAcceleration <= InputVector.Size())
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, FString::Printf(TEXT("IsMovementWithinThresholds Returns True")));
		return true;
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("IsMovementWithinThresholds Returns False")));
		return false;
	}
}
