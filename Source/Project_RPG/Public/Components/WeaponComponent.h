// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
/* Structures */
#include "Structures/WeaponProperties.h"
#include "Structures/WeaponMontages.h"
#include "WeaponComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_RPG_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:

	/// <summary>
	/// Getter Functions 
	
	UFUNCTION()
	FORCEINLINE EWeaponType GetWeaponType() { return WeaponProperties.WeaponType; }

	UFUNCTION()
	FORCEINLINE FName GetOneHandedHandSocketName() { return WeaponProperties.OneHandedHandSocket; }

	UFUNCTION()
	FORCEINLINE FName GetTwoHandedHandSocketName() { return WeaponProperties.TwoHandedHandSocket; }

	UFUNCTION()
	FORCEINLINE FName GetHipWeaponHolsterSocketName() { return WeaponProperties.HipWeaponHolsterSocket; }

	UFUNCTION()
	FORCEINLINE FName GetBackWeaponHolsterSocketName() { return WeaponProperties.BackWeaponHolsterSocket; }

	/// Getter Functions 
	/// </summary>

public:

	/* References */
	UPROPERTY(VisibleAnywhere, Category = "References")
	class ACharacter* OwnerCharacter = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "References")
	class AActor* OwnerActor = nullptr;

	UPROPERTY()
	bool bDrawWeapon = false;
		
public:

	UFUNCTION()
	void SetOwnerAsPlayer();

	/**
	* Play Montage Functions
	*/

	UFUNCTION()
	void PlayMontageFromSection(UAnimMontage* Montage, const FName& SectionName = FName(TEXT("Default")));

	UFUNCTION(BlueprintCallable)
	void PlayDrawWeaponMontage();

	UFUNCTION(BlueprintCallable)
	void PlayHolsterWeaponMontage();

	UFUNCTION(BlueprintCallable)
	void PlayAttackMontage();

	UFUNCTION()
	void PlayDodgeMontage();

	/**
	* Weapon Hit Trace Functions
	*/

	UFUNCTION()
	FHitResult BoxTrace();

	UFUNCTION()
	void ClearIgnoreActors() { IgnoreActors.Empty(); }

	/**
	* Weapon Particle Effect Functions
	*/

	UFUNCTION()
	void SpawnWeaponTrailEffect();

	UFUNCTION()
	void DespawnWeaponTrailEffect();


	/**
	* Combo Functions
	*/



private:

	/**
	* Weapon
	*/

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	FWeaponProperties WeaponProperties;

	UPROPERTY(EditAnywhere, Category = "Weapon Standard Montages")
	FWeaponMontages WeaponStandardMontages;

	/**
	* Animation Montages
	*/

	UPROPERTY(EditAnywhere, Category = "Weapon Anim Montages")
	class UAnimMontage* AttackMontage = nullptr;

	/**
	* Trace Properties
	*/

	UPROPERTY(VisibleAnywhere, Category = "Trace Properties")
	class UStaticMeshComponent* OwnerStaticMesh = nullptr;

	UPROPERTY(EditAnywhere, Category = "Trace Properties")
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;

	UPROPERTY(VisibleAnywhere, Category = "Trace Properties")
	TArray<AActor*> IgnoreActors;

	UPROPERTY(EditAnywhere, Category = "Trace Properties")
	FName TraceStartSocketName = FName("TraceStart");

	UPROPERTY(EditAnywhere, Category = "Trace Properties")
	FName TraceEndSocketName = FName("TraceEnd");

	/**
	* Particle Effects
	*/

	UPROPERTY(VisibleAnywhere, Category = "Weapon: Trail Effects")
	class UParticleSystemComponent* WeaponTrailEffectSystem = nullptr;

	UPROPERTY(EditAnywhere, Category = "Weapon: Trail Effects")
	class UParticleSystem* WeaponTrailEffect = nullptr;

};
