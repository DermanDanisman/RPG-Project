// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CharacterMovementDataComponent.h"
/* Game Framework */
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

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
	
}

// Called every frame
void UCharacterMovementDataComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UCharacterMovementDataComponent::SetMovementMode(ELocomotionState LocomotionState)
{
	if (Character)
	{
		switch (LocomotionState)
		{
		case ELocomotionState::Idle:
			Character->GetCharacterMovement()->MaxWalkSpeed = 0.0f;
			break;
		case ELocomotionState::Walking:
			Character->GetCharacterMovement()->MaxWalkSpeed = WalkingSpeed;
			break;
		case ELocomotionState::Jogging:
			Character->GetCharacterMovement()->MaxWalkSpeed = JoggingSpeed;
			break;
		case ELocomotionState::Sprinting:
			Character->GetCharacterMovement()->MaxWalkSpeed = SprintingSpeed;
			break;
		case ELocomotionState::EW_MAX:
			break;
		default:
			break;
		}
	}
}

