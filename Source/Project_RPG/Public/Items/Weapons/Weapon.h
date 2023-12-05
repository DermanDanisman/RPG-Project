// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Weapon.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	EWT_OneHandedSword UMETA(DisplayName = "One-Handed Sword"),
	EWT_TwoHandedSword UMETA(DisplayName = "Two-Handed Sword")
};

UCLASS()
class PROJECT_RPG_API AWeapon : public AItem
{
	GENERATED_BODY()

public:

	AWeapon();

	UFUNCTION()
	FORCEINLINE EWeaponType GetWeaponType() const { return WeaponType; }

	UFUNCTION()
	FORCEINLINE UCharacterWeaponComponent* GetCharacterWeaponComponent() const { return CharacterWeaponComponent; }

public:

	UFUNCTION()
	void AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName);

	UFUNCTION()
	void EquipWeapon(USceneComponent* InParent, FName InSocketName);

	UFUNCTION(BlueprintCallable)
	void DrawWeapon(USceneComponent* InParent, FName InSocketName);

	UFUNCTION(BlueprintCallable)
	void HolsterWeapon(USceneComponent* InParent, FName InSocketName);
	
protected:

	/* Delegate Binded Functions */
	/* Now, here's a very important detail. These functions are declared in Item class and they're declared with UFUNCTION macros. Now, we can't have these UFUNCTION macros on the override.
	We have to remove them, otherwise we'll get compilation errors. The fact that these functions are UFUNCTIONs is an inherited quality. 
	So it's like there's a UFUNCTION macro there, we just can't see it that UFUNCTION is inherited. So we need to make sure that our overrides do not have UFUNCTION on them. */
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

private:

	UPROPERTY(VisibleAnywhere)
	class UCharacterWeaponComponent* CharacterWeaponComponent;

	UPROPERTY(EditAnywhere, Category = WeaponType)
	EWeaponType WeaponType = EWeaponType::EWT_OneHandedSword;


};
