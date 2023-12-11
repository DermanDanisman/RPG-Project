// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/PlayerCharacterController.h"
/* Enhanced Input */
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
/* Game Framework */
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
/* Kismet */
#include "Kismet/KismetMathLibrary.h"
/* Interfaces*/
#include "Interfaces/PlayerInputInterface.h"


APlayerCharacterController::APlayerCharacterController()
{
}

void APlayerCharacterController::BeginPlay()
{
	Super::BeginPlay();

	/* Adding Mapping Context */
	UEnhancedInputLocalPlayerSubsystem* InputLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (InputLocalPlayerSubsystem)
	{
		InputLocalPlayerSubsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	ControlledCharacter = Cast<ACharacter>(GetPawn());

	PlayerInputInterface = Cast<IPlayerInputInterface>(ControlledCharacter);
}

void APlayerCharacterController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayerCharacterController::SetupInputComponent()
{
	Super::SetupInputComponent();

	/* Binding Input Actions to their respective functions */
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	if (EnhancedInputComponent)
	{
		// Moving
		EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &APlayerCharacterController::Move);
		// Looking
		EnhancedInputComponent->BindAction(IA_Look, ETriggerEvent::Triggered, this, &APlayerCharacterController::Look); 
		// Jumping
		EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Started, this, &APlayerCharacterController::Jump);
		// Jogging
		EnhancedInputComponent->BindAction(IA_Jog, ETriggerEvent::Started, this, &APlayerCharacterController::Jog);
		// Start Sprinting
		EnhancedInputComponent->BindAction(IA_Sprint, ETriggerEvent::Triggered, this, &APlayerCharacterController::StartSprint);
		// Stop Sprinting
		EnhancedInputComponent->BindAction(IA_Sprint, ETriggerEvent::Completed, this, &APlayerCharacterController::StopSprint);
		EnhancedInputComponent->BindAction(IA_Sprint, ETriggerEvent::Canceled, this, &APlayerCharacterController::StopSprint);
		// Crouching
		EnhancedInputComponent->BindAction(IA_Crouch, ETriggerEvent::Started, this, &APlayerCharacterController::Crouch);
		// Pickup
		EnhancedInputComponent->BindAction(IA_Pickup, ETriggerEvent::Started, this, &APlayerCharacterController::Pickup);
		// Attack Or Equip Weapon
		EnhancedInputComponent->BindAction(IA_Attack, ETriggerEvent::Started, this, &APlayerCharacterController::Attack);
		// Draw or Holster Weapon
		EnhancedInputComponent->BindAction(IA_HolsterDrawWeapon, ETriggerEvent::Started, this, &APlayerCharacterController::DrawWeapon);

	}
}

// Input Function responsible for character movement
void APlayerCharacterController::Move(const FInputActionValue& Value)
{
	const FVector2D MoveDirectionValue = Value.Get<FVector2D>();

	if (ControlledCharacter)
	{
		const FRotator Rotation = GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		//FRotator ForwardMakeRot = UKismetMathLibrary::MakeRotator(0.f, 0.f, Rotation.Yaw);
		//FVector ForwardDirection = UKismetMathLibrary::GetForwardVector(ForwardMakeRot);
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		GetPawn()->AddMovementInput(ForwardDirection, MoveDirectionValue.Y);

		//FRotator RightMakeRot = UKismetMathLibrary::MakeRotator(0.f, 0.f, Rotation.Yaw);
		//FVector RightDirection = UKismetMathLibrary::GetRightVector(RightMakeRot);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		GetPawn()->AddMovementInput(RightDirection, MoveDirectionValue.X);
	}
	else
	{
		const FVector ForwardDirection = GetPawn()->GetActorForwardVector();
		GetPawn()->AddMovementInput(ForwardDirection, MoveDirectionValue.Y);

		const FVector RightDirection = GetPawn()->GetActorRightVector();
		GetPawn()->AddMovementInput(RightDirection, MoveDirectionValue.X);
	}
}

// Input Function responsible for character camera movement
void APlayerCharacterController::Look(const FInputActionValue& Value)
{
	const FVector2D LookDirectionValue = Value.Get<FVector2D>();

	GetPawn()->AddControllerYawInput(LookDirectionValue.X);
	GetPawn()->AddControllerPitchInput(LookDirectionValue.Y);
}

// Input Function responsible for characters jumping
void APlayerCharacterController::Jump(const FInputActionValue& Value)
{
	const bool bShouldJump = Value.Get<bool>();
	if (ControlledCharacter->GetClass()->ImplementsInterface(UPlayerInputInterface::StaticClass()))
	{
		if (PlayerInputInterface)
		{
			PlayerInputInterface->PII_Jump();
		}
		//IPlayerInputInterface::Execute_PII_Jump(ControlledCharacter, bShouldJump);
	}
}

// Input Function responsible for characters jogging
void APlayerCharacterController::Jog(const FInputActionValue& Value)
{
	const bool bShouldJog = Value.Get<bool>();
	if (ControlledCharacter->GetClass()->ImplementsInterface(UPlayerInputInterface::StaticClass()))
	{
		if (PlayerInputInterface)
		{
			PlayerInputInterface->PII_Jog();
		}
		//IPlayerInputInterface::Execute_PII_Jog(ControlledCharacter, bShouldJog);
	}
	//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, FString::Printf(TEXT("Jog On")));
}

// Input Function responsible for characters sprinting
void APlayerCharacterController::StartSprint(const FInputActionValue& Value)
{
	const bool bShouldSprint = Value.Get<bool>();
	if (ControlledCharacter->GetClass()->ImplementsInterface(UPlayerInputInterface::StaticClass()))
	{
		if (PlayerInputInterface)
		{
			PlayerInputInterface->PII_StartSprint();
		}
		//IPlayerInputInterface::Execute_PII_StartSprint(ControlledCharacter, bShouldSprint);
	}
	//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, FString::Printf(TEXT("Sprinting")));
}

void APlayerCharacterController::StopSprint(const FInputActionValue& Value)
{
	const bool bShouldSprint = Value.Get<bool>();
	if (ControlledCharacter->GetClass()->ImplementsInterface(UPlayerInputInterface::StaticClass()))
	{
		if (PlayerInputInterface)
		{
			PlayerInputInterface->PII_StopSprint();
		}
		//IPlayerInputInterface::Execute_PII_StopSprint(ControlledCharacter, bShouldSprint);
	}
}

// Input Function responsible for characters crouching
void APlayerCharacterController::Crouch(const FInputActionValue& Value)
{
	const bool bShouldCrouch = Value.Get<bool>();
	if (ControlledCharacter->GetClass()->ImplementsInterface(UPlayerInputInterface::StaticClass()))
	{
		if (PlayerInputInterface)
		{
			PlayerInputInterface->PII_Crouch();
		}
		//IPlayerInputInterface::Execute_PII_Crouch(ControlledCharacter, bShouldCrouch);
	}
}

// Input Function responsible for characters pickup function
void APlayerCharacterController::Pickup(const FInputActionValue& Value)
{
	const bool bShouldPickup = Value.Get<bool>();
	if (ControlledCharacter->GetClass()->ImplementsInterface(UPlayerInputInterface::StaticClass()))
	{
		if (PlayerInputInterface)
		{
			PlayerInputInterface->PII_Pickup();
		}
		//IPlayerInputInterface::Execute_PII_Pickup(ControlledCharacter, bShouldPickup);
	}
	//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, FString::Printf(TEXT("Pickup")));
}

// Input Function responsible for characters attack function
void APlayerCharacterController::Attack(const FInputActionValue& Value)
{
	const bool bShouldAttack = Value.Get<bool>();
	if (ControlledCharacter->GetClass()->ImplementsInterface(UPlayerInputInterface::StaticClass()))
	{
		if (PlayerInputInterface)
		{
			PlayerInputInterface->PII_Attack();
		}
		//IPlayerInputInterface::Execute_PII_Attack(ControlledCharacter, bShouldAttack);
	}
}

// Input Function responsible for characters weapon draw and holster functions
void APlayerCharacterController::DrawWeapon(const FInputActionValue& Value)
{
	const bool bShouldDraw = Value.Get<bool>();
	if (ControlledCharacter->GetClass()->ImplementsInterface(UPlayerInputInterface::StaticClass()))
	{
		if (PlayerInputInterface)
		{
			PlayerInputInterface->PII_DrawWeapon();
		}
		//IPlayerInputInterface::Execute_PII_DrawWeapon(ControlledCharacter, bShouldDraw);
	}
}

