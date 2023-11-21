// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PlayerCharacter.h"
/* Game Framework */
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
/* Camera */
#include "Camera/CameraComponent.h"
/* Components */
#include "Components/SkeletalMeshComponent.h"
/* Hair Strands Core */
#include "GroomComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->JumpZVelocity = JumpVelocity;
	GetCharacterMovement()->MaxWalkSpeed = WalkingSpeed;
	GetCharacterMovement()->MaxWalkSpeedCrouched = CrouchingSpeed;


	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	CameraSpringArm->SetupAttachment(GetRootComponent());
	CameraSpringArm->bUsePawnControlRotation = true;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(CameraSpringArm);

	HairMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HairMesh"));
	HairMesh->SetupAttachment(GetMesh());

	HairGroomComponent = CreateDefaultSubobject<UGroomComponent>(TEXT("HairGroomComponent"));
	HairGroomComponent->SetupAttachment(GetMesh());
	HairGroomComponent->AttachmentName = FString("head");

	EyebrowsGroomComponent = CreateDefaultSubobject<UGroomComponent>(TEXT("EyebrowsGroomComponent"));
	EyebrowsGroomComponent->SetupAttachment(GetMesh());
	EyebrowsGroomComponent->AttachmentName = FString("head");
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
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
	if (GetCharacterMovement())
	{
		if (!bJogging)
		{
			GetCharacterMovement()->MaxWalkSpeed = 350.f;
			bJogging = true;
		}
		else
		{
			GetCharacterMovement()->MaxWalkSpeed = 150.f;
			bJogging = false;
		}
	}
}

void APlayerCharacter::PII_StartSprint_Implementation(bool bShouldSprint)
{
	if (GetCharacterMovement())
	{
		if (!bIsCrouched) GetCharacterMovement()->MaxWalkSpeed = 500.f;
	}
}

void APlayerCharacter::PII_StopSprint_Implementation(bool bShouldSprint)
{
	if (GetCharacterMovement())
	{
		if (bJogging) GetCharacterMovement()->MaxWalkSpeed = 350.f;
		else GetCharacterMovement()->MaxWalkSpeed = 150.f;
	}
}

void APlayerCharacter::PII_Crouch_Implementation(bool bShouldCrouch)
{
	if (!bIsCrouched) Crouch();
	else if (bIsCrouched) UnCrouch();
}
/// Movement Input Functions
/// </summary>
