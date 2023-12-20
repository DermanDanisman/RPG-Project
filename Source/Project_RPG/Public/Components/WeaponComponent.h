// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
/* Structures */
#include "Structures/WeaponProperties.h"
#include "Structures/WeaponMontages.h"
#include "Structures/WeaponData.h"
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
	FORCEINLINE UStaticMesh* GetWeaponMesh() { return WeaponData->WeaponProperties.WeaponMesh; }

	UFUNCTION()
	FORCEINLINE EWeaponType GetWeaponType() { return WeaponData->WeaponProperties.WeaponType; }

	UFUNCTION()
	FORCEINLINE FName GetWeaponSocketName() { return WeaponData->WeaponProperties.WeaponSocket; }

	UFUNCTION()
	FORCEINLINE FName GetHipWeaponHolsterSocketName() { return WeaponData->WeaponProperties.HipWeaponHolsterSocket; }

	UFUNCTION()
	FORCEINLINE FName GetBackWeaponHolsterSocketName() { return WeaponData->WeaponProperties.BackWeaponHolsterSocket; }

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

	UFUNCTION()
	void SetWeaponDataName();

	UFUNCTION()
	void SetWeaponData();

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
	* Weapon Data
	*/

	UPROPERTY(VisibleAnywhere, Category = "Weapon Data")
	FName WeaponDataName;

	UPROPERTY(EditAnywhere, Category = "Weapon Data")
	UDataTable* WeaponDataTable = nullptr;

	//UPROPERTY(EditAnywhere, Category = "Weapon Data")
	FWeaponData* WeaponData = nullptr;

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
