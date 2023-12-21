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

	UFUNCTION(Category = "Movement Inputs")
	virtual void PII_Jump() = 0;

	UFUNCTION(Category = "Movement Inputs")
	virtual void PII_Jog() = 0;

	UFUNCTION(Category = "Movement Inputs")
	virtual void PII_StartSprint() = 0;

	UFUNCTION(Category = "Movement Inputs")
	virtual void PII_StopSprint() = 0;

	UFUNCTION(Category = "Movement Inputs")
	virtual void PII_Crouch() = 0;

	UFUNCTION(Category = "Item Interaction Inputs")
	virtual void PII_Pickup() = 0;

	UFUNCTION(Category = "Weapon Interaction Inputs")
	virtual void PII_Attack() = 0;

	UFUNCTION(Category = "Weapon Interaction Inputs")
	virtual void PII_DrawWeapon() = 0;

	UFUNCTION(Category = "Weapon Interaction Inputs")
	virtual void PII_FocusOnTarget() = 0;

	UFUNCTION(Category = "Mapping Context")
	virtual void PII_AddInputMappingContext(const UInputMappingContext* InputMappingContext) = 0;

	UFUNCTION(Category = "Mapping Context")
	virtual void PII_RemoveInputMappingContext(const UInputMappingContext* InputMappingContext) = 0;
};
