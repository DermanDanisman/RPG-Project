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
#include "Components/ChildActorComponent.h"
/* Hair Strands Core */
#include "GroomComponent.h"
/* Enums */
#include "Enums/LocomotionState.h"
/* Actor Components */
#include "Components/CharacterMovementDataComponent.h"
#include "Components/WeaponComponent.h"
#include "Components/CharacterMontageComponent.h"
#include "Components/CharacterInventoryComponent.h"
/* Enhanced Input */
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
/* Item */
#include "Items/Item.h"
#include "Items/Weapons/Weapon.h"
/* Animation */
#include "Animation/AnimMontage.h"
/* Kismet */
#include "Kismet/KismetSystemLibrary.h"
/* Targeting System */
#include "Actors/TargetingSystem/TargetingSystem.h"



// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/* Creating Actor Components */
	CharacterMovementDataComponent = CreateDefaultSubobject<UCharacterMovementDataComponent>(TEXT("CharacterMovementDataComponent"));
	CharacterMovementDataComponent->SetComponentTickEnabled(false);

	CharacterMontageComponent = CreateDefaultSubobject<UCharacterMontageComponent>(TEXT("CharacterMontageComponent"));
	CharacterMontageComponent->SetComponentTickEnabled(false);

	CharacterInventoryComponent = CreateDefaultSubobject<UCharacterInventoryComponent>(TEXT("CharacterInventoryComponent"));
	CharacterInventoryComponent->SetComponentTickEnabled(false);

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

	/* Targeting System */
	// Create and attach the child actor component
	TargetingSystemComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("TargetingSystemComponent"));
	TargetingSystemComponent->SetupAttachment(RootComponent); // Attach to the root component or another appropriate component
	// Set the child actor class to your TargetingSystem class
	TargetingSystemComponent->SetChildActorClass(ATargetingSystem::StaticClass()); // Replace ATargetingSystem with your TargetingSystem class
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<APlayerController>(GetController());

	GetCharacterMovement()->JumpZVelocity = CharacterMovementDataComponent->GetJumpVelocity();
	GetCharacterMovement()->MaxWalkSpeed = CharacterMovementDataComponent->GetWalkSpeed();
	GetCharacterMovement()->MaxWalkSpeedCrouched = CharacterMovementDataComponent->GetCrouchSpeed();

	CharacterMovementDataComponent->SetMovementMode(ELocomotionState::ELS_Jogging, CharacterState);
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
void APlayerCharacter::PII_Jump()
{
	if (GrabbedWeapon)
	{
		if (CharacterState != ECharacterState::ECS_Unequipped && ActionState == EActionState::EAS_Unoccupied)
		{
			GrabbedWeapon->Dodge();
			ActionState = EActionState::EAS_Dodging;
			CharacterMovementDataComponent->SetMovementMode(ELocomotionState::ELS_Jogging, CharacterState);
		}
	}
	//Jump();
}

void APlayerCharacter::PII_Jog()
{
	if (CharacterMovementDataComponent)
	{
		if (!CharacterMovementDataComponent->GetJoggingBool())
		{
			CharacterMovementDataComponent->SetMovementMode(ELocomotionState::ELS_Jogging, CharacterState);
			CharacterMovementDataComponent->SetJoggingBool(true);
		}
		else
		{
			CharacterMovementDataComponent->SetMovementMode(ELocomotionState::ELS_Walking, CharacterState);
			CharacterMovementDataComponent->SetJoggingBool(false);
		}
	}
}

void APlayerCharacter::PII_StartSprint()
{
	if (CharacterMovementDataComponent)
	{
		if (!bIsCrouched) CharacterMovementDataComponent->SetMovementMode(ELocomotionState::ELS_Sprinting, CharacterState);
	}
}

void APlayerCharacter::PII_StopSprint()
{
	if (CharacterMovementDataComponent)
	{
		CharacterMovementDataComponent->SetJoggingBool(true);

		if (GetCharacterMovement())
		{
			if (CharacterMovementDataComponent->GetJoggingBool())
			{
				CharacterMovementDataComponent->SetMovementMode(ELocomotionState::ELS_Jogging, CharacterState);
			}
			else
			{
				CharacterMovementDataComponent->SetMovementMode(ELocomotionState::ELS_Walking, CharacterState);
			}
		}
	}
}

void APlayerCharacter::PII_Crouch()
{
	if (!bIsCrouched) Crouch();
	else if (bIsCrouched) UnCrouch();
}

/// Movement Input Functions
/// </summary>

/// <summary>
/// Item Interaction Input Functions
void APlayerCharacter::PII_Pickup()
{
	if (OverlappingItem)
	{
		AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);
		if (OverlappingWeapon)
		{
			if (OverlappingWeapon->GetWeaponType() == EWeaponType::EWT_OneHandedSword)
			{
				OverlappingWeapon->PickupWeapon(GetMesh(), OverlappingWeapon->GetWeaponHolsterSocketName());
			}
			else if (OverlappingWeapon->GetWeaponType() == EWeaponType::EWT_TwoHandedSword)
			{
				OverlappingWeapon->PickupWeapon(GetMesh(), OverlappingWeapon->GetWeaponHolsterSocketName());
			}
			OverlappingWeapon->SetOwner(this);
			CharacterInventoryComponent->AddItemToInventory(OverlappingWeapon);
			GrabbedWeapon = Cast<AWeapon>(CharacterInventoryComponent->GetLastPickedUpWeapon());
			GrabbedWeapon->GetWeaponComponent()->SetOwnerAsPlayer();
			SetOverlappingItem(nullptr);
		}
	}
}

/// Item Interaction Input Functions
/// </summary>

/// <summary>
/// Action Input Functions

void APlayerCharacter::PII_DrawWeapon()
{
	if (ActionState == EActionState::EAS_Unoccupied)
	{
		if (GrabbedWeapon)
		{
			if (!GrabbedWeapon->GetWeaponComponent()->bDrawWeapon)
			{
				GrabbedWeapon->GetWeaponComponent()->PlayDrawWeaponMontage();
				ActionState = EActionState::EAS_DrawingWeapon;
			}
			else
			{
				GrabbedWeapon->GetWeaponComponent()->PlayHolsterWeaponMontage();
				ActionState = EActionState::EAS_DrawingWeapon;
			}
		}
	}
}

void APlayerCharacter::PII_Attack()
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, FString::Printf(TEXT("Attack")));
	if (ActionState == EActionState::EAS_Unoccupied)
	{
		if (GrabbedWeapon)
		{
			GrabbedWeapon->GetWeaponComponent()->PlayAttackMontage();
			ActionState = EActionState::EAS_Attacking;
		}
	}
}

void APlayerCharacter::PII_FocusOnTarget()
{
	ATargetingSystem* TargetSystem = Cast<ATargetingSystem>(TargetingSystemComponent->GetChildActor());
	if (TargetSystem)
	{
		TargetSystem->GetTargetsInRange();
		AActor* TargetInSight = TargetSystem->GetTargetInLineOfSight(TargetSystem->GetTargetList());

		if (bLockedOnTarget && TargetInSight == TargetSystem->GetCurrentTarget())
		{
			//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, FString::Printf(TEXT("")));
			// If currently locked onto a target and it's the same as the one in line of sight, unlock
			TargetSystem->ClearCurrentTarget();
			bLockedOnTarget = false;
		}
		else if (TargetInSight)
		{
			// If there is a different target in line of sight, lock onto it
			TargetSystem->SelectTarget(TargetInSight);
			bLockedOnTarget = true;
		}
		else
		{
			TargetSystem->ClearCurrentTarget();
		}
		// If no target in line of sight and not currently locked on, do nothing
	}
}

void APlayerCharacter::PII_FocusedTargetCycle(float InputValue)
{
	if (bLockedOnTarget)
	{	
		ATargetingSystem* TargetSystem = Cast<ATargetingSystem>(TargetingSystemComponent->GetChildActor());
		if (TargetSystem)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, FString::Printf(TEXT("Focused Target Cycle Function")));
			TargetSystem->GetNextTarget(InputValue);
		}
	}
}

/// Action Input Functions
/// </summary>

/// <summary>
/// Add and Remove Input Mapping Context Functions
void APlayerCharacter::PII_AddInputMappingContext(const UInputMappingContext* InputMappingContext)
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

void APlayerCharacter::PII_RemoveInputMappingContext(const UInputMappingContext* InputMappingContext)
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

/// <summary>
/// Getting References
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

/// Getting References
/// </summary>

/// <summary>
/// Pure C++ Interface Usage
/// Called When a Weapon Hits this character, this used for playing correct HitMontages when this character gets hit.
/// </summary>
void APlayerCharacter::WI_GetWeaponHit(const FVector& ImpactPoint)
{
	if (CharacterMontageComponent)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("GetWeaponHit")));
		DrawDebugSphere(GetWorld(), ImpactPoint, 10.f, 12, FColor::Red, false, 5.0f);
		CharacterMontageComponent->PlayHitReactionMontage(ImpactPoint);
	}
}

// Setting Character State by weapon type
void APlayerCharacter::SetCharacterState(ECharacterState CharacterStateEnum)
{
	switch (CharacterStateEnum)
	{
	case ECharacterState::ECS_Unequipped:
	{
		CharacterState = ECharacterState::ECS_Unequipped;
		CharacterMovementDataComponent->SetCharacterMovementRotationSettings(false, false, 0.f);
		break;
	}
	case ECharacterState::ECS_OneHandedSword: 
	{
		if (GrabbedWeapon)
		{
			if (GrabbedWeapon->GetWeaponType() == EWeaponType::EWT_OneHandedSword)
			{
				CharacterState = ECharacterState::ECS_OneHandedSword;
				CharacterMovementDataComponent->SetCharacterMovementRotationSettings(false, true, 360.f);
			}
		}
		break;
	}
	case ECharacterState::ECS_TwoHandedSword:
		if (GrabbedWeapon)
		{
			if (GrabbedWeapon->GetWeaponType() == EWeaponType::EWT_TwoHandedSword)
			{
				CharacterState = ECharacterState::ECS_TwoHandedSword;
				CharacterMovementDataComponent->SetCharacterMovementRotationSettings(false, true, 360.f);
			}
		}
		break;
	case ECharacterState::ECS_SwordAndShield:
		if (GrabbedWeapon)
		{
			if (GrabbedWeapon->GetWeaponType() == EWeaponType::EWT_SwordAndShield)
			{
				CharacterState = ECharacterState::ECS_SwordAndShield;
				CharacterMovementDataComponent->SetCharacterMovementRotationSettings(false, true, 360.f);
			}
		}
		break;
	case ECharacterState::ECS__MAX:
		break;
	default:
		break;
	}
}

