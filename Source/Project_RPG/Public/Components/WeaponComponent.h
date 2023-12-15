// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
/* Interfaces */
#include "Interfaces/ReferencesInterface.h"

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

	/* References */
	UPROPERTY(VisibleAnywhere, Category = "References")
	class ACharacter* OwnerCharacter = nullptr;

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
	UFUNCTION()
	void PlayMontageFromSection(UAnimMontage* Montage, const FName& SectionName = FName(TEXT("Default")));

	UFUNCTION(BlueprintCallable)
	void PlayDrawWeaponMontage();

	UFUNCTION(BlueprintCallable)
	void PlayHolsterWeaponMontage();

	UFUNCTION(BlueprintCallable)
	void PlayAttackMontage();

	/**
	* Weapon Hit Trace Function
	*/
	UFUNCTION()
	FHitResult BoxTrace();

	UFUNCTION()
	void ClearIgnoreActors() { IgnoreActors.Empty(); }

private:

	/**
	* Animation Montages
	*/

	UPROPERTY(EditAnywhere, Category = "Weapon Anim Montages")
	class UAnimMontage* DrawWeaponMontage = nullptr;

	UPROPERTY(EditAnywhere, Category = "Weapon Anim Montages")
	class UAnimMontage* HolsterWeaponMontage = nullptr;

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


};
