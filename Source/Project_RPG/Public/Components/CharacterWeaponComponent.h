// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
/* Interfaces */
#include "Interfaces/ReferencesInterface.h"

#include "CharacterWeaponComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_RPG_API UCharacterWeaponComponent : public UActorComponent
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

	UFUNCTION()
	void PlayHitReactionMontage(const FName& SectionName);

	UFUNCTION()
	FHitResult BoxTrace();

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

	UPROPERTY(EditAnywhere, Category = "Hit Reaction Montages")
	class UAnimMontage* HitReactionMontage = nullptr;

	/**
	* Trace Properties
	*/

	UPROPERTY(VisibleAnywhere, Category = "Trace Properties")
	class USkeletalMeshComponent* SkeletalMesh = nullptr;

	UPROPERTY(EditAnywhere, Category = "Trace Properties")
	TSubclassOf<USkeletalMeshComponent> SkeletalMeshClass;

	UPROPERTY(EditAnywhere, Category = "Trace Properties")
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;

	UPROPERTY(EditAnywhere, Category = "Trace Properties")
	FName TraceStartSocketName = FName("TraceStart");

	UPROPERTY(EditAnywhere, Category = "Trace Properties")
	FName TraceEndSocketName = FName("TraceEnd");


};
