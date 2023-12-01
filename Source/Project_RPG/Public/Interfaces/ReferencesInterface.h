// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ReferencesInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UReferencesInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECT_RPG_API IReferencesInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Player Character References")
	APlayerCharacter* RI_GetPlayerCharacter() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Player Character References")
	APlayerController* RI_GetPlayerController() const;
};
