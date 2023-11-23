// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
/* Interfaces*/
#include "Interfaces/PlayerInputInterface.h"

#include "PlayerCharacter.generated.h"

UCLASS()
class PROJECT_RPG_API APlayerCharacter : public ACharacter, public IPlayerInputInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:	

	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* CameraSpringArm;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* ViewCamera;

	UPROPERTY(VisibleAnywhere)
	class USkeletalMeshComponent* HairMesh;

	UPROPERTY(VisibleAnywhere)
	class UGroomComponent* HairGroomComponent;

	UPROPERTY(VisibleAnywhere)
	class UGroomComponent* EyebrowsGroomComponent;

private:

	// Interface Functions

	/// <summary>
	/// Movement Input Interface Functions
	
	UPROPERTY(EditAnywhere, Category = "Movement Properties")
	bool bJogging = false;

	UPROPERTY(EditAnywhere, Category = "Movement Properties")
	float JumpVelocity = 520.f;

	UPROPERTY(EditAnywhere, Category = "Movement Properties")
	float WalkingSpeed = 147.f;

	UPROPERTY(EditAnywhere, Category = "Movement Properties")
	float JoggingSpeed = 427.f;

	UPROPERTY(EditAnywhere, Category = "Movement Properties")
	float SprintingSpeed = 627.f;

	UPROPERTY(EditAnywhere, Category = "Movement Properties")
	float CrouchingSpeed = 247.f;

	UFUNCTION()
	virtual void PII_Jump_Implementation(bool bShouldJump) override;

	UFUNCTION()
	virtual void PII_Jog_Implementation(bool bShouldJog) override;

	UFUNCTION()
	virtual void PII_StartSprint_Implementation(bool bShouldSprint) override;
	UFUNCTION()
	virtual void PII_StopSprint_Implementation(bool bShouldSprint) override;

	UFUNCTION()
	virtual void PII_Crouch_Implementation(bool bShouldCrouch) override;
	UPROPERTY()
	bool bCrouching = false;

	/// Movement Input Interface Functions
	/// </summary>
};
