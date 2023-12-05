// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
/* Interfaces */
#include "Interfaces/ReferencesInterface.h"

#include "CharacterWeaponComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_RPG_API UCharacterWeaponComponent : public UActorComponent, public IReferencesInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterWeaponComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:

	/* References */
	UPROPERTY(VisibleAnywhere, Category = "References")
	class ACharacter* Character = nullptr;

	UPROPERTY()
	bool bDrawWeapon = false;

	UPROPERTY()
	bool bHolsterWeapon = true;
		
public:

	UFUNCTION()
	void SetOwnerAsPlayer();

	/**
	* Play Montage Functions
	*/

	UFUNCTION(BlueprintCallable)
	void PlayDrawWeaponMontage();

	UFUNCTION(BlueprintCallable)
	void PlayHolsterWeaponMontage();

	UFUNCTION(BlueprintCallable)
	void PlayAttackMontage();

private:

	/**
	* Animation Montages
	*/

	UPROPERTY(EditAnywhere, Category = "Weapon Anim Montages")
	class UAnimMontage* DrawWeaponMontage;

	UPROPERTY(EditAnywhere, Category = "Weapon Anim Montages")
	class UAnimMontage* HolsterWeaponMontage;

	UPROPERTY(EditAnywhere, Category = "Weapon Anim Montages")
	class UAnimMontage* AttackMontage;

};
