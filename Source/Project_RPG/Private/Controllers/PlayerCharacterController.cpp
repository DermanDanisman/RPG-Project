// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/PlayerCharacterController.h"
/* Enhanced Input */
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
/* Game Framework */
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
/* Components */
#include "Components/CapsuleComponent.h"


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

	MoveToFloor();
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
		// Sprinting
		EnhancedInputComponent->BindAction(IA_Sprint, ETriggerEvent::Triggered, this, &APlayerCharacterController::Sprint);
		// Crouching
		EnhancedInputComponent->BindAction(IA_Crouch, ETriggerEvent::Started, this, &APlayerCharacterController::Crouch);
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
	if (ControlledCharacter)
	{
		ControlledCharacter->Jump();
	}
}

// Input Function responsible for characters jogging
void APlayerCharacterController::Jog(const FInputActionValue& Value)
{
	const bool bShouldJog = Value.Get<bool>();
	//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, FString::Printf(TEXT("Jog On")));
}

// Input Function responsible for characters sprinting
void APlayerCharacterController::Sprint(const FInputActionValue& Value)
{
	const bool bShouldSprint = Value.Get<bool>();
	//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, FString::Printf(TEXT("Sprinting")));
}

// Input Function responsible for characters crouching
void APlayerCharacterController::Crouch(const FInputActionValue& Value)
{
	const bool bShouldCrouch = Value.Get<bool>();
}

// Function for Moving character to the floor at the begining of the game
void APlayerCharacterController::MoveToFloor()
{
	if (ControlledCharacter)
	{
		float InitialStepHeight = ControlledCharacter->GetCharacterMovement()->MaxStepHeight;
		float LargeStepHeight = 2000.f;
		ControlledCharacter->GetCharacterMovement()->MaxStepHeight = LargeStepHeight;

		FFindFloorResult FindFloorResult;
		ControlledCharacter->GetCharacterMovement()->FindFloor(ControlledCharacter->GetCapsuleComponent()->GetComponentLocation(), FindFloorResult, false);

		FVector FindFloorResultVector = FVector(0.f, 0.f, FindFloorResult.FloorDist) + ControlledCharacter->GetCapsuleComponent()->GetComponentLocation();

		ControlledCharacter->TeleportTo(FindFloorResultVector, ControlledCharacter->GetActorRotation());
		ControlledCharacter->GetCharacterMovement()->MaxStepHeight = InitialStepHeight;

		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, FString::Printf(TEXT("Move To Floor Executed")));
	}

}

