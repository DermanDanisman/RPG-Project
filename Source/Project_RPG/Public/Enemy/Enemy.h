// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/WeaponInterface.h"
#include "Enemy.generated.h"

UCLASS()
class PROJECT_RPG_API AEnemy : public ACharacter, public IWeaponInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

private:


private:	

	/// <summary>
	/// Weapon Interface Functions
	
	UFUNCTION()
	virtual void WI_GetWeaponHit(const FVector& ImpactPoint) override;

	/// Weapon Interface Functions
	/// </summary>
};
