// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/PlayerCharacterController.h"
/* Enhanced Input */
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
/* Game Framework */
#include "GameFramework/Character.h"


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

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		GetPawn()->AddMovementInput(ForwardDirection, MoveDirectionValue.Y);

		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		GetPawn()->AddMovementInput(RightDirection, MoveDirectionValue.X);
	}

	const FVector ForwardDirection = GetPawn()->GetActorForwardVector();
	GetPawn()->AddMovementInput(ForwardDirection, MoveDirectionValue.Y);

	const FVector RightDirection = GetPawn()->GetActorRightVector();
	GetPawn()->AddMovementInput(RightDirection, MoveDirectionValue.X);
}

// Input Function responsible for character camera movement
void APlayerCharacterController::Look(const FInputActionValue& Value)
{
	const FVector2D LookDirectionValue = Value.Get<FVector2D>();


	GetPawn()->AddControllerYawInput(LookDirectionValue.X);

	GetPawn()->AddControllerPitchInput(LookDirectionValue.Y);
}

void APlayerCharacterController::Jump(const FInputActionValue& Value)
{
	if (ControlledCharacter)
	{
		ControlledCharacter->Jump();
	}
}

