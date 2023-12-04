// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerInputInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPlayerInputInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECT_RPG_API IPlayerInputInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Movement Inputs")
	void PII_Jump(bool bShouldJump);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Movement Inputs")
	void PII_Jog(bool bShouldJog);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Movement Inputs")
	void PII_StartSprint(bool bShouldSprint);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Movement Inputs")
	void PII_StopSprint(bool bShouldSprint);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Movement Inputs")
	void PII_Crouch(bool bShouldCrouch);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Item Interaction Inputs")
	void PII_Pickup(bool bShouldPickup);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Weapon Interaction Inputs")
	void PII_Attack(bool bShouldAttack);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Weapon Interaction Inputs")
	void PII_DrawWeapon(bool bShouldDraw);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Weapon Interaction Inputs")
	void PII_HolsterWeapon(bool bShouldHolster);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Mapping Context")
	void PII_AddInputMappingContext(UInputMappingContext* InputMappingContext);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Mapping Context")
	void PII_RemoveInputMappingContext(UInputMappingContext* InputMappingContext);
};
