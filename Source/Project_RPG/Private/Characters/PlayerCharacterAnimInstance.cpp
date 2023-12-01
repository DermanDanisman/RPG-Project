// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PlayerCharacterAnimInstance.h"
#include "Characters/PlayerCharacter.h"
/* Game Framework*/
#include "GameFramework/CharacterMovementComponent.h"
/* Kismet */
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
/* Animation */
#include "Animation/CachedAnimData.h"

void UPlayerCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	SetReferences();
	SetCachedAnimStateData();
}

void UPlayerCharacterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	DeltaTimeX = DeltaTime;

	if (PlayerCharacter)
	{
		SetEssentialMovementData();
		DetermineLocomotionState();
		UpdateLocomotionValues(FName("MoveData_Speed"));


		if (LocomotionState == ELocomotionState::ELS_Idle)
		{
			bWalkOnEntryFlag = false;
			bJogOnEntryFlag = false;
		}
		else if (LocomotionState == ELocomotionState::ELS_Walking)
		{
			if (!bWalkOnEntryFlag)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, FString::Printf(TEXT("bWalkOnEntryFlag")));
				UpdateOnWalkEntry();
				bWalkOnEntryFlag = true;
				bJogOnEntryFlag = false;
				bSprintOnEntryFlag = false;
			}
		}
		else if (LocomotionState == ELocomotionState::ELS_Jogging)
		{
			if (!bJogOnEntryFlag)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, FString::Printf(TEXT("bJogOnEntryFlag")));
				UpdateOnJogEntry();
				bJogOnEntryFlag = true;
				bWalkOnEntryFlag = false;
				bSprintOnEntryFlag = false;
			}
		}
		else if (LocomotionState == ELocomotionState::ELS_Sprinting)
		{
			if (!bSprintOnEntryFlag)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, FString::Printf(TEXT("bSprintOnEntryFlag")));
				UpdateOnSprintEntry();
				bJogOnEntryFlag = false;
				bWalkOnEntryFlag = false;
				bSprintOnEntryFlag = true;
			}
		}
	}
}

void UPlayerCharacterAnimInstance::NativePostEvaluateAnimation()
{
	if (PlayerCharacter && !bOrientRotationToMovement && CharacterState == ECharacterState::ECS_Unequipped)
	{
		// Disable Character Rotation When Strafing
		UpdateCharacterRotation();
		ResetTransitions();
	}
}

void UPlayerCharacterAnimInstance::SetReferences()
{
	AnimInstance = Cast<UAnimInstance>(this);
	PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());
	if (PlayerCharacter)
	{
		CharacterMovementComponent = Cast<UCharacterMovementComponent>(PlayerCharacter->GetCharacterMovement());
	}
}

void UPlayerCharacterAnimInstance::SetCachedAnimStateData()
{
	LocomotionStateData.StateMachineName = FName("Main States");
	LocomotionStateData.StateName = FName("On Ground");

	WalkStateData.StateMachineName = FName("Ground Locomotion");
	WalkStateData.StateName = FName("Walk");

	JogStateData.StateMachineName = FName("Ground Locomotion");
	JogStateData.StateName = FName("Jog");

	SprintStateData.StateMachineName = FName("Ground Locomotion");
	SprintStateData.StateName = FName("Sprint");
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
		bUseControllerDesiredRotation = CharacterMovementComponent->bUseControllerDesiredRotation;
		CharacterYawRotationRate = CharacterMovementComponent->RotationRate.Yaw;
		CharacterState = PlayerCharacter->GetCharacterState();
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
				LocomotionState = ELocomotionState::ELS_Idle;
			}
			else
			{
				if (IsMovementWithinThresholds(350.f, 500.f, 0.75f)) LocomotionState = ELocomotionState::ELS_Sprinting;
				else if (IsMovementWithinThresholds(1.0f, 300.f, 0.5f)) LocomotionState = ELocomotionState::ELS_Jogging;
				else if (IsMovementWithinThresholds(1.0f, 0.0f, 0.01f)) LocomotionState = ELocomotionState::ELS_Walking;
				else LocomotionState = ELocomotionState::ELS_Idle;
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
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("PlayRate: %f "), PlayRate));
}

/* Updates target rotations and other variables when the character starts walking. It calculates angles and sets flags for walk start animations.*/
void UPlayerCharacterAnimInstance::UpdateOnWalkEntry()
{
	if (GroundSpeed < 50.f)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, FString::Printf(TEXT("UpdateOnWalkEntry")));
		StartRotation = PlayerCharacter->GetActorRotation();
		PrimaryTargetRotation = UKismetMathLibrary::MakeRotFromX(InputVector);
		SecondaryTargetRotation = UKismetMathLibrary::MakeRotFromX(InputVector);
		WalkStartAngle = UKismetMathLibrary::NormalizedDeltaRotator(PrimaryTargetRotation, StartRotation).Yaw;
		bPlayWalkStart = true;
	}
	else
	{
		ResetTargetRotations();
	}
}

/* Similar to UpdateOnWalkEntry, but for jogging. It updates target rotations and flags when the character starts jogging.*/
void UPlayerCharacterAnimInstance::UpdateOnJogEntry()
{
	if (GroundSpeed < 200.f)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("Update On Jog Entry")));
		StartRotation = PlayerCharacter->GetActorRotation();
		PrimaryTargetRotation = UKismetMathLibrary::MakeRotFromX(InputVector);
		SecondaryTargetRotation = UKismetMathLibrary::MakeRotFromX(InputVector);
		JogStartAngle = UKismetMathLibrary::NormalizedDeltaRotator(PrimaryTargetRotation, StartRotation).Yaw;
		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("Jog Start Angle: %f "), JogStartAngle));
		bPlayJogStart = true;
	}
	else
	{
		ResetTargetRotations();
	}
}

/* Similar to UpdateOnWalkEntry, but for sprinting. It updates target rotations and flags when the character starts sprinting.*/
void UPlayerCharacterAnimInstance::UpdateOnSprintEntry()
{
	if (GroundSpeed < 400.f)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("Update On Sprint Entry")));
		StartRotation = PlayerCharacter->GetActorRotation();
		PrimaryTargetRotation = UKismetMathLibrary::MakeRotFromX(InputVector);
		SecondaryTargetRotation = UKismetMathLibrary::MakeRotFromX(InputVector);
		SprintStartAngle = UKismetMathLibrary::NormalizedDeltaRotator(PrimaryTargetRotation, StartRotation).Yaw;
		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("Jog Start Angle: %f "), JogStartAngle));
		bPlaySprintStart = true;
	}
	else
	{
		ResetTargetRotations();
	}

}

/* Resets the target rotations to the character's current rotation. This is used to reset the orientation after specific movements or animations.*/
void UPlayerCharacterAnimInstance::ResetTargetRotations()
{
	PrimaryTargetRotation = SecondaryTargetRotation = PlayerCharacter->GetActorRotation();
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("ResetTargetRotations")));
}

FRotator UPlayerCharacterAnimInstance::GetTargetRotation()
{
	FRotator ReturnValue;
	if (bDoInputVectorRotation)
	{
		ReturnValue = FRotator(0.f, UKismetMathLibrary::MakeRotFromX(InputVector).Yaw, 0.f);
	}
	else
	{
		ReturnValue = FRotator(0.f, UKismetMathLibrary::MakeRotFromX(Velocity).Yaw, 0.f);
	}

	return ReturnValue;
}

/* Updates character rotation according to the playing animation // OrientRotationToMovement should be turned off and Rotation should be set to 0 in CharacterMovementComponent */
void UPlayerCharacterAnimInstance::UpdateCharacterRotation()
{
	if (!PlayerCharacter->HasAnyRootMotion() && LocomotionStateData.IsMachineRelevant(*AnimInstance))
	{
		switch (LocomotionState)
		{
		case ELocomotionState::ELS_Idle:
			return;
			break;
		case ELocomotionState::ELS_Walking:
		{
			PrimaryTargetRotation = UKismetMathLibrary::RInterpTo_Constant(PrimaryTargetRotation, GetTargetRotation(), DeltaTimeX, 1000.f);
			SecondaryTargetRotation = UKismetMathLibrary::RInterpTo(SecondaryTargetRotation, PrimaryTargetRotation, DeltaTimeX, 10.f);

			float NormalizedWalkRotationDelta = UKismetMathLibrary::SafeDivide(GetCurveValue(FName("MoveData_WalkRotationDelta")), WalkStateData.GetGlobalWeight(*AnimInstance)); //MoveData_WalkRotationDelta
			float CharacterRotationYaw = SecondaryTargetRotation.Yaw + NormalizedWalkRotationDelta;
			FRotator CharacterRotation = FRotator(SecondaryTargetRotation.Pitch, CharacterRotationYaw, SecondaryTargetRotation.Roll); //UKismetMathLibrary::MakeRotator(SecondaryTargetRotation.Roll, SecondaryTargetRotation.Pitch, CharacterRotationYaw);

			PlayerCharacter->SetActorRotation(CharacterRotation);
			break;
		}
		case ELocomotionState::ELS_Jogging:
		{
			PrimaryTargetRotation = UKismetMathLibrary::RInterpTo_Constant(PrimaryTargetRotation, GetTargetRotation(), DeltaTimeX, 1000.f);
			SecondaryTargetRotation = UKismetMathLibrary::RInterpTo(SecondaryTargetRotation, PrimaryTargetRotation, DeltaTimeX, 10.f);

			float JogNormalizedWalkRotationDelta = UKismetMathLibrary::SafeDivide(GetCurveValue(FName("MoveData_JogRotationDelta")), JogStateData.GetGlobalWeight(*AnimInstance));
			float JogCharacterRotationYaw = SecondaryTargetRotation.Yaw + JogNormalizedWalkRotationDelta;
			FRotator JogCharacterRotation = FRotator(SecondaryTargetRotation.Pitch, JogCharacterRotationYaw, SecondaryTargetRotation.Roll); //UKismetMathLibrary::MakeRotator(SecondaryTargetRotation.Roll, SecondaryTargetRotation.Pitch, CharacterRotationYaw);

			PlayerCharacter->SetActorRotation(JogCharacterRotation);
			break;
		}
		case ELocomotionState::ELS_Sprinting:
		{
			PrimaryTargetRotation = UKismetMathLibrary::RInterpTo_Constant(PrimaryTargetRotation, GetTargetRotation(), DeltaTimeX, 1000.f);
			SecondaryTargetRotation = UKismetMathLibrary::RInterpTo(SecondaryTargetRotation, PrimaryTargetRotation, DeltaTimeX, 10.f);

			// MoveData_JogRotationDelta needs to be change when proper Sprinting animations come
			float SprintNormalizedWalkRotationDelta = UKismetMathLibrary::SafeDivide(GetCurveValue(FName("MoveData_JogRotationDelta")), SprintStateData.GetGlobalWeight(*AnimInstance));
			float SprintCharacterRotationYaw = SecondaryTargetRotation.Yaw + SprintNormalizedWalkRotationDelta;
			FRotator SprintCharacterRotation = FRotator(SecondaryTargetRotation.Pitch, SprintCharacterRotationYaw, SecondaryTargetRotation.Roll); //UKismetMathLibrary::MakeRotator(SecondaryTargetRotation.Roll, SecondaryTargetRotation.Pitch, CharacterRotationYaw);

			PlayerCharacter->SetActorRotation(SprintCharacterRotation);
			break;
		}
		case ELocomotionState::ELS_MAX:
			return;
			break;
		}
	}
	else
	{
		ResetTargetRotations();
	}
}

void UPlayerCharacterAnimInstance::ResetTransitions()
{
	bPlayWalkStart = false;
	bPlayJogStart = false;
}
