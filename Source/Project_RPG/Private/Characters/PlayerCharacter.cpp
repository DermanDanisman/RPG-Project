// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PlayerCharacter.h"
#include "Controllers/PlayerCharacterController.h"
/* Game Framework */
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
/* Camera */
#include "Camera/CameraComponent.h"
/* Components */
#include "Components/SkeletalMeshComponent.h"
/* Hair Strands Core */
#include "GroomComponent.h"
/* Enums */
#include "Enums/LocomotionState.h"
/* Actor Components */
#include "Components/CharacterMovementDataComponent.h"
#include "Components/CharacterWeaponComponent.h"
/* Enhanced Input */
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
/* Item */
#include "Items/Item.h"
#include "Items/Weapons/Weapon.h"
/* Animation */
#include "Animation/AnimMontage.h"



// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/* Creating Actor Components */
	CharacterMovementDataComponent = CreateDefaultSubobject<UCharacterMovementDataComponent>(TEXT("CharacterMovementDataComponent"));
	CharacterMovementDataComponent->SetComponentTickEnabled(false);

	CharacterWeaponComponent = CreateDefaultSubobject<UCharacterWeaponComponent>(TEXT("CharacterWeaponComponent"));

	/* Control Settings */
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	CharacterMovementDataComponent->SetCharacterMovementRotationSettings(false, false, 0.0f);

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

	PlayerController = Cast<APlayerController>(GetController());

	GetCharacterMovement()->JumpZVelocity = CharacterMovementDataComponent->GetJumpVelocity();
	GetCharacterMovement()->MaxWalkSpeed = CharacterMovementDataComponent->GetWalkSpeed();
	GetCharacterMovement()->MaxWalkSpeedCrouched = CharacterMovementDataComponent->GetCrouchSpeed();
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
	if (CharacterMovementDataComponent)
	{
		if (!CharacterMovementDataComponent->GetJoggingBool())
		{
			CharacterMovementDataComponent->SetMovementMode(ELocomotionState::ELS_Jogging);
			CharacterMovementDataComponent->SetJoggingBool(true);
		}
		else
		{
			CharacterMovementDataComponent->SetMovementMode(ELocomotionState::ELS_Walking);
			CharacterMovementDataComponent->SetJoggingBool(false);
		}
	}
}

void APlayerCharacter::PII_StartSprint_Implementation(bool bShouldSprint)
{
	if (CharacterMovementDataComponent)
	{
		if (!bIsCrouched) CharacterMovementDataComponent->SetMovementMode(ELocomotionState::ELS_Sprinting);
	}
}

void APlayerCharacter::PII_StopSprint_Implementation(bool bShouldSprint)
{
	if (CharacterMovementDataComponent)
	{
		CharacterMovementDataComponent->SetJoggingBool(true);

		if (GetCharacterMovement())
		{
			if (CharacterMovementDataComponent->GetJoggingBool())
			{
				CharacterMovementDataComponent->SetMovementMode(ELocomotionState::ELS_Jogging);
			}
			else
			{
				CharacterMovementDataComponent->SetMovementMode(ELocomotionState::ELS_Walking);
			}
		}
	}
}

void APlayerCharacter::PII_Crouch_Implementation(bool bShouldCrouch)
{
	if (!bIsCrouched) Crouch();
	else if (bIsCrouched) UnCrouch();
}

/// Movement Input Functions
/// </summary>


/// <summary>
/// Action Input Functions
void APlayerCharacter::PII_AttackOrDrawWeapon_Implementation(bool bShouldAttack)
{
	if (GrabbedWeapon)
	{
		if (GrabbedWeapon->GetWeaponType() == EWeaponType::EWT_OneHandedSword)
		{
			CharacterWeaponComponent->PlayDrawWeaponMontage(DrawOneHandedSwordMontage);
		}
	}
}

void APlayerCharacter::PII_HolsterWeapon_Implementation(bool bShouldHolster)
{
	if (GrabbedWeapon)
	{
		if (GrabbedWeapon->GetWeaponType() == EWeaponType::EWT_OneHandedSword)
		{
			CharacterWeaponComponent->PlayHolsterWeaponMontage(HolsterOneHandedSwordMontage);
		}
	}
}

/// Action Input Functions
/// </summary>


/// <summary>
/// Item Interaction Input Functions
void APlayerCharacter::PII_Pickup_Implementation(bool bShouldPickup)
{
	if (OverlappingItem)
	{
		AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);
		if (OverlappingWeapon)
		{
			OverlappingWeapon->Equip(GetMesh(), FName("WeaponHolsterSocket"));
			GrabbedWeapon = OverlappingWeapon;
			SetOverlappingItem(nullptr);
		}
	}
}

/// Item Interaction Input Functions
/// </summary>


/// <summary>
/// Add and Remove Input Mapping Context Functions
void APlayerCharacter::PII_AddInputMappingContext_Implementation(UInputMappingContext* InputMappingContext)
{
	if (PlayerController)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (Subsystem)
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
}

void APlayerCharacter::PII_RemoveInputMappingContext_Implementation(UInputMappingContext* InputMappingContext)
{
	if (PlayerController)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (Subsystem)
		{
			Subsystem->RemoveMappingContext(InputMappingContext);
		}
	}
}

/// Add and Remove Input Mapping Context Functions
/// </summary>


APlayerCharacter* APlayerCharacter::RI_GetPlayerCharacter_Implementation() const
{
	return const_cast<APlayerCharacter*>(this);
}

APlayerController* APlayerCharacter::RI_GetPlayerController_Implementation() const
{
	return PlayerController;
}

AWeapon* APlayerCharacter::RI_GetPlayerGrabbedWeapon_Implementation() const
{
	if (GrabbedWeapon) return GrabbedWeapon;
	else return nullptr;
}

void APlayerCharacter::SetCharacterState(ECharacterState CharacterStateEnum)
{
	if (CharacterStateEnum == ECharacterState::ECS_Unequipped)
	{
		CharacterState = ECharacterState::ECS_Unequipped;
		CharacterMovementDataComponent->SetCharacterMovementRotationSettings(false, false, 0.f);
	}
	else
	{
		if (GrabbedWeapon)
		{
			if (GrabbedWeapon->GetWeaponType() == EWeaponType::EWT_OneHandedSword)
			{
				CharacterState = ECharacterState::ECS_OneHandedSword;
				CharacterMovementDataComponent->SetCharacterMovementRotationSettings(false, true, 360.f);
			}
		}
	}

}

