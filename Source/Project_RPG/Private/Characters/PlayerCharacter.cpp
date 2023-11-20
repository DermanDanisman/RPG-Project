// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PlayerCharacter.h"
/* Game Framework */
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
/* Camera */
#include "Camera/CameraComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMovementComponent()->NavAgentProps.bCanCrouch = true;

	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("USpringArmComponent"));
	CameraSpringArm->SetupAttachment(GetRootComponent());

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("UCameraComponent"));
	ViewCamera->SetupAttachment(CameraSpringArm);
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// Get Character Movement Component
	CharacterMovementComponent = Cast<UCharacterMovementComponent>(GetCharacterMovement());
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

/// <summary>
/// Movement Input Functions
void APlayerCharacter::PII_Jump_Implementation(bool bShouldJump)
{
	Jump();
}

void APlayerCharacter::PII_Jog_Implementation(bool bShouldJog)
{
	if (CharacterMovementComponent)
	{
		if (!bJogging)
		{
			CharacterMovementComponent->MaxWalkSpeed = 350.f;
			bJogging = true;
		}
		else
		{
			CharacterMovementComponent->MaxWalkSpeed = 150.f;
			bJogging = false;
		}
	}
}

void APlayerCharacter::PII_StartSprint_Implementation(bool bShouldSprint)
{
	if (CharacterMovementComponent)
	{
		if (!bIsCrouched) CharacterMovementComponent->MaxWalkSpeed = 500.f;
	}
}

void APlayerCharacter::PII_StopSprint_Implementation(bool bShouldSprint)
{
	if (CharacterMovementComponent)
	{
		if (bJogging) CharacterMovementComponent->MaxWalkSpeed = 350.f;
		else CharacterMovementComponent->MaxWalkSpeed = 150.f;
	}
}

void APlayerCharacter::PII_Crouch_Implementation(bool bShouldCrouch)
{
	if (!bIsCrouched) Crouch();
	else if (bIsCrouched) UnCrouch();
}
/// Movement Input Functions
/// </summary>
