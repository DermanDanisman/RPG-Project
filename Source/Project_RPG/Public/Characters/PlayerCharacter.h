// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
/* Interfaces*/
#include "Interfaces/PlayerInputInterface.h"
#include "Interfaces/ReferencesInterface.h"
/* Enums */
#include "Enums/CharacterState.h"
#include "PlayerCharacter.generated.h"


UCLASS()
class PROJECT_RPG_API APlayerCharacter : public ACharacter, public IPlayerInputInterface, public IReferencesInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:

	UFUNCTION()
	FORCEINLINE void SetOverlappingItem(AItem* Item) { OverlappingItem = Item; }

	UFUNCTION()
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE AWeapon* GetGrabbedWeapon() { return GrabbedWeapon; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:	

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class USpringArmComponent* CameraSpringArm;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class UCameraComponent* ViewCamera;

	UPROPERTY(VisibleAnywhere)
	class USkeletalMeshComponent* HairMesh;

	UPROPERTY(VisibleAnywhere)
	class UGroomComponent* HairGroomComponent;

	UPROPERTY(VisibleAnywhere)
	class UGroomComponent* EyebrowsGroomComponent;

	UPROPERTY(VisibleAnywhere)
	class UCharacterMovementDataComponent* CharacterMovementDataComponent;

	UPROPERTY(VisibleAnywhere)
	class UCharacterWeaponComponent* CharacterWeaponComponent;

private:

	/* References */
	UPROPERTY()
	class APlayerController* PlayerController;

private:

	UPROPERTY(VisibleInstanceOnly, Category = "References")
	class AItem* OverlappingItem;

	UPROPERTY(VisibleInstanceOnly, Category = "References")
	class AWeapon* GrabbedWeapon;

	UPROPERTY(VisibleInstanceOnly, Category = "References")
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

	UPROPERTY(EditAnywhere, Category = "One-Handed Sword Anim Montages")
	class UAnimMontage* DrawOneHandedSwordMontage;

	UPROPERTY(EditAnywhere, Category = "One-Handed Sword Anim Montages")
	class UAnimMontage* HolsterOneHandedSwordMontage;

private:

	UFUNCTION(BlueprintCallable)
	void SetCharacterState(ECharacterState CharacterStateEnum);

private:

	// Interface Functions

	/// <summary>
	/// Movement Input Interface Functions

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

	/// Movement Input Interface Functions
	/// </summary>
	
	/// <summary>
	/// Action Input Interface Functions
	
	UFUNCTION()
	virtual void PII_AttackOrDrawWeapon_Implementation(bool bShouldAttack) override;

	UFUNCTION()
	virtual void PII_HolsterWeapon_Implementation(bool bShouldHolster) override;
	/// Action Input Interface Functions
	/// </summary>
	
	/// <summary>
	/// Item Input Interface Functions 
	
	UFUNCTION()
	virtual void PII_Pickup_Implementation(bool bShouldPickup) override;

	/// Item Input Interface Functions 
	/// </summary>	
	
	/// <summary>
	/// Input Mapping Context Manupilation Functions

	UFUNCTION()
	virtual void PII_AddInputMappingContext_Implementation(UInputMappingContext* InputMappingContext);

	UFUNCTION()
	virtual void PII_RemoveInputMappingContext_Implementation(UInputMappingContext* InputMappingContext);
	
	/// Input Mapping Context Manupilation Functions
	/// </summary>

	/// <summary>
	/// References Interface Functions

	UFUNCTION()
	virtual APlayerCharacter* RI_GetPlayerCharacter_Implementation() const override;

	UFUNCTION()
	virtual APlayerController* RI_GetPlayerController_Implementation() const override;

	UFUNCTION()
	virtual AWeapon* RI_GetPlayerGrabbedWeapon_Implementation() const override;

	/// References Interface Functions
	/// </summary>
};
