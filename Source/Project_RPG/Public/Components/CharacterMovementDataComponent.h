// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
/* Enums */
#include "Enums/LocomotionState.h"
#include "CharacterMovementDataComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_RPG_API UCharacterMovementDataComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterMovementDataComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:

	UFUNCTION(BlueprintCallable)
	void SetMovementMode(ELocomotionState LocomotionState);

	/* Getters and Setters */
	UFUNCTION()
	float GetJumpVelocity() { return JumpVelocity; }

	UFUNCTION()
	float GetWalkSpeed() { return WalkingSpeed; }

	UFUNCTION()
	float GetCrouchSpeed() { return CrouchingSpeed; }

	UFUNCTION()
	void SetJoggingBool(bool bShouldJog) { bJogging = bShouldJog; }

	UFUNCTION()
	bool GetJoggingBool() { return bJogging; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:

	/* References */
	class ACharacter* Character;

	/* Movement Essential Data */

	UPROPERTY(VisibleAnywhere, Category = "Movement Essential Data")
	bool bJogging = false;

	UPROPERTY(EditAnywhere, Category = "Movement Essential Data")
	float JumpVelocity = 520.f;

	UPROPERTY(EditAnywhere, Category = "Movement Essential Data")
	float WalkingSpeed = 147.f;

	UPROPERTY(EditAnywhere, Category = "Movement Essential Data")
	float JoggingSpeed = 427.f;

	UPROPERTY(EditAnywhere, Category = "Movement Essential Data")
	float SprintingSpeed = 627.f;

	UPROPERTY(EditAnywhere, Category = "Movement Essential Data")
	float CrouchingSpeed = 247.f;

		
};