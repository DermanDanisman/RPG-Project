// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
/* Enhanced Input */
#include "InputActionValue.h"
#include "PlayerCharacterController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_RPG_API APlayerCharacterController : public APlayerController
{
	GENERATED_BODY()

public:
	// Sets default values 
	APlayerCharacterController();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupInputComponent() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


protected: // Enhanced Input Section
	
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputMappingContext* DefaultMappingContext = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* IA_Move = nullptr;;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* IA_Look = nullptr;;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* IA_Jump = nullptr;;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* IA_Jog = nullptr;;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* IA_Sprint = nullptr;;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* IA_Crouch = nullptr;;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* IA_Pickup = nullptr;;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* IA_Attack = nullptr;;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* IA_HolsterDrawWeapon = nullptr;;

	UFUNCTION()
	void Move(const FInputActionValue& Value);

	UFUNCTION()
	void Look(const FInputActionValue& Value);

	UFUNCTION()
	void Jump(const FInputActionValue& Value);

	UFUNCTION()
	void Jog(const FInputActionValue& Value);

	UFUNCTION()
	void StartSprint(const FInputActionValue& Value);

	UFUNCTION()
	void StopSprint(const FInputActionValue& Value);

	UFUNCTION()
	void Crouch(const FInputActionValue& Value);

	UFUNCTION()
	void Pickup(const FInputActionValue& Value);

	UFUNCTION()
	void Attack(const FInputActionValue& Value);

	UFUNCTION()
	void DrawWeapon(const FInputActionValue& Value);

private: // References
	
	UPROPERTY()
	class ACharacter* ControlledCharacter = nullptr;

	class IPlayerInputInterface* PlayerInputInterface;
};
