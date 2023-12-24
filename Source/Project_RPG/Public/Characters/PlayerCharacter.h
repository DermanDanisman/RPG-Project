// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
/* Interfaces*/
#include "Interfaces/PlayerInputInterface.h"
#include "Interfaces/ReferencesInterface.h"
#include "Interfaces/WeaponInterface.h"
/* Enums */
#include "Enums/CharacterState.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class PROJECT_RPG_API APlayerCharacter : public ACharacter, public IPlayerInputInterface, public IReferencesInterface, public IWeaponInterface
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

	UFUNCTION()
	FORCEINLINE EActionState GetActionState() const { return ActionState; }

	UFUNCTION()
	FORCEINLINE void SetActionState(EActionState NewActionState) { ActionState = NewActionState; }

	UFUNCTION(BlueprintCallable)
	void SetCharacterState(ECharacterState CharacterStateEnum);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:	

	UPROPERTY(VisibleAnywhere, Category = "Character Properties")
	class USpringArmComponent* CameraSpringArm;

	UPROPERTY(VisibleAnywhere, Category = "Character Properties")
	class UCameraComponent* ViewCamera;

	UPROPERTY(VisibleAnywhere, Category = "Character Properties")
	class USkeletalMeshComponent* HairMesh;

	UPROPERTY(VisibleAnywhere, Category = "Character Properties")
	class UGroomComponent* HairGroomComponent;

	UPROPERTY(VisibleAnywhere, Category = "Character Properties")
	class UGroomComponent* EyebrowsGroomComponent;

	UPROPERTY(VisibleAnywhere, Category = "Character Components")
	class UCharacterMovementDataComponent* CharacterMovementDataComponent;

	UPROPERTY(VisibleAnywhere, Category = "Character Components")
	class UCharacterMontageComponent* CharacterMontageComponent;

	UPROPERTY(VisibleAnywhere, Category = "Character Components")
	class UCharacterInventoryComponent* CharacterInventoryComponent;

	

private:

	/* References */
	UPROPERTY()
	class APlayerController* PlayerController;

private:

	UPROPERTY(VisibleInstanceOnly, Category = "References")
	class AItem* OverlappingItem;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "References")
	class AWeapon* GrabbedWeapon;

	UPROPERTY(VisibleInstanceOnly, Category = "Player State Enums")
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

	UPROPERTY(VisibleInstanceOnly, Category = "Player State Enums")
	EActionState ActionState = EActionState::EAS_Unoccupied;

private:

	// Child Actor Component to hold the Targeting System
	UPROPERTY(VisibleAnywhere, Category = "Targeting System")
	UChildActorComponent* TargetingSystemComponent;

	UPROPERTY()
	bool bLockedOnTarget = false;


private:

	// Interface Functions

	/// <summary>
	/// Movement Input Interface Functions

	UFUNCTION()
	virtual void PII_Jump() override;

	UFUNCTION()
	virtual void PII_Jog() override;

	UFUNCTION()
	virtual void PII_StartSprint() override;
	UFUNCTION()
	virtual void PII_StopSprint() override;

	UFUNCTION()
	virtual void PII_Crouch() override;

	/// Movement Input Interface Functions
	/// </summary>
	
	/// <summary>
	/// Item Input Interface Functions 
	
	UFUNCTION()
	virtual void PII_Pickup() override;

	/// Item Input Interface Functions 
	/// </summary>	
	
	/// <summary>
	/// Action Input Interface Functions

	UFUNCTION()
	virtual void PII_DrawWeapon() override;

	UFUNCTION()
	virtual void PII_Attack() override;

	UFUNCTION()
	virtual void PII_FocusOnTarget() override;

	UFUNCTION()
	virtual void PII_FocusedTargetCycle(float InputValue) override;
	/// Action Input Interface Functions
	/// </summary>

	
	/// <summary>
	/// Input Mapping Context Manupilation Functions

	UFUNCTION()
	virtual void PII_AddInputMappingContext(const UInputMappingContext* InputMappingContext);

	UFUNCTION()
	virtual void PII_RemoveInputMappingContext(const UInputMappingContext* InputMappingContext);
	
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
	
	/// <summary>
	/// Weapon Interface Functions

	UFUNCTION()
	virtual void WI_GetWeaponHit(const FVector& ImpactPoint) override;

	/// Weapon Interface Functions
	/// </summary>
};
