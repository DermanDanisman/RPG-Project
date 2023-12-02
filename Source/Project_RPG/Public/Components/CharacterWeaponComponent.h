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
	bool bDrawWeapon;

	UPROPERTY()
	bool bHolsterWeapon;
		
public:

	UFUNCTION(BlueprintCallable)
	void PlayDrawWeaponMontage(UAnimMontage* WeaponDrawMontage);

	UFUNCTION(BlueprintCallable)
	void PlayHolsterWeaponMontage(UAnimMontage* WeaponHolsterMontage);
};
