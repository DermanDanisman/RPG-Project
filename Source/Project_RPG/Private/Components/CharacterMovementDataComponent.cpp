// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CharacterMovementDataComponent.h"
/* Game Framework */
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
/* Components */
#include "Components/CapsuleComponent.h"
#include "Structures/CharacterSpeedData.h"


// Sets default values for this component's properties
UCharacterMovementDataComponent::UCharacterMovementDataComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

}

// Called when the game starts
void UCharacterMovementDataComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<ACharacter>(GetOwner());

	MoveToFloor();
	
}

// Called every frame
void UCharacterMovementDataComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

// Function for Moving character to the floor at the begining of the game
void UCharacterMovementDataComponent::MoveToFloor()
{
	if (Character)
	{
		float InitialStepHeight = Character->GetCharacterMovement()->MaxStepHeight;
		float LargeStepHeight = 2000.f;
		Character->GetCharacterMovement()->MaxStepHeight = LargeStepHeight;

		FFindFloorResult FindFloorResult;
		Character->GetCharacterMovement()->FindFloor(Character->GetCapsuleComponent()->GetComponentLocation(), FindFloorResult, false);

		FVector FindFloorResultVector = FVector(0.f, 0.f, FindFloorResult.FloorDist) + Character->GetCapsuleComponent()->GetComponentLocation();

		if (FindFloorResult.bBlockingHit)
		{
			Character->TeleportTo(FindFloorResultVector, Character->GetActorRotation());
			Character->GetCharacterMovement()->MaxStepHeight = InitialStepHeight;
			//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, FString::Printf(TEXT("Move To Floor Executed")));
		}
	}
}

void UCharacterMovementDataComponent::SetMovementMode(ELocomotionState LocomotionState, ECharacterState CharacterState)
{
	if (Character)
	{
		if (CharacterSpeedDataTable)
		{
			/* When you call FindRow on a DataTable to retrieve a row by its name, the function needs to know what to do if it doesn't find the row you're asking for.
			This is where ContextString comes into play. If the row is not found, Unreal Engine will log an error message, and the ContextString will be included in this message.*/
			static const FString ContextString(TEXT("Character Speed Context"));
			FCharacterSpeedData* UnequippedSpeedData = CharacterSpeedDataTable->FindRow<FCharacterSpeedData>(FName(TEXT("UnequippedSpeedData")), ContextString);
			FCharacterSpeedData* EquippedSpeedData = CharacterSpeedDataTable->FindRow<FCharacterSpeedData>(FName(TEXT("EquippedSpeedData")), ContextString);
			
			if (CharacterState == ECharacterState::ECS_Unequipped)
			{
				SetMovementSpeed(LocomotionState, UnequippedSpeedData);
			}
			else
			{
				SetMovementSpeed(LocomotionState, EquippedSpeedData);
			}
		}
	}
}

void UCharacterMovementDataComponent::SetMovementSpeed(ELocomotionState LocomotionState, FCharacterSpeedData* UnequippedSpeedData)
{
	switch (LocomotionState)
	{
	case ELocomotionState::ELS_Idle:
		Character->GetCharacterMovement()->MaxWalkSpeed = 0.0f;
		break;
	case ELocomotionState::ELS_Walking:
		Character->GetCharacterMovement()->MaxWalkSpeed = UnequippedSpeedData->WalkingSpeed;
		break;
	case ELocomotionState::ELS_Jogging:
		Character->GetCharacterMovement()->MaxWalkSpeed = UnequippedSpeedData->JoggingSpeed;
		break;
	case ELocomotionState::ELS_Sprinting:
		Character->GetCharacterMovement()->MaxWalkSpeed = UnequippedSpeedData->SprintingSpeed;
		break;
	case ELocomotionState::ELS_MAX:
		break;
	default:
		break;
	}
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, FString::Printf(TEXT("Movement Speed: %f"), Character->GetCharacterMovement()->MaxWalkSpeed));
}


void UCharacterMovementDataComponent::SetCharacterMovementRotationSettings(bool bOrientRotationToMovement, bool bUseControllerDesiredRotation, float YawRotationRate)
{
	if (Character)
	{
		Character->GetCharacterMovement()->bOrientRotationToMovement = bOrientRotationToMovement;
		Character->GetCharacterMovement()->bUseControllerDesiredRotation = bUseControllerDesiredRotation;
		Character->GetCharacterMovement()->RotationRate = FRotator(0.f, YawRotationRate, 0.f);
	}
}

