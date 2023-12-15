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
	EWT_TwoHandedSword UMETA(DisplayName = "Two-Handed Sword"),
	EWT_SwordAndShield UMETA(DisplayName = "Sword and Shield")
};

UCLASS()
class PROJECT_RPG_API AWeapon : public AItem
{
	GENERATED_BODY()

public:

	AWeapon();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	FORCEINLINE EWeaponType GetWeaponType() const { return WeaponType; }

	UFUNCTION()
	FORCEINLINE UWeaponComponent* GetCharacterWeaponComponent() const { return WeaponComponent; }

	UFUNCTION()
	FORCEINLINE FName GetWeaponHolsterSocketName() const { return WeaponHolsterSocketName; }

	UFUNCTION()
	FORCEINLINE FName GetWeaponHandSocketName() const { return WeaponHandSocketName; }

public:

	UFUNCTION()
	void AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName);

	UFUNCTION()
	void PickupWeapon(USceneComponent* InParent, FName InSocketName);

	UFUNCTION(BlueprintCallable)
	void DrawWeapon(USceneComponent* InParent, FName InSocketName);

	UFUNCTION(BlueprintCallable)
	void HolsterWeapon(USceneComponent* InParent, FName InSocketName);

	UFUNCTION()
	void SetWeaponBoxCollisionEnabled();

	UFUNCTION()
	void SetWeaponBoxCollisionDisabled();
	
protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/* Delegate Binded Functions */
	/* Now, here's a very important detail. These functions are declared in Item class and they're declared with UFUNCTION macros. Now, we can't have these UFUNCTION macros on the override.
	We have to remove them, otherwise we'll get compilation errors. The fact that these functions are UFUNCTIONs is an inherited quality. 
	So it's like there's a UFUNCTION macro there, we just can't see it that UFUNCTION is inherited. So we need to make sure that our overrides do not have UFUNCTION on them. */
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	UFUNCTION()
	void OnWeaponBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	class UBoxComponent* WeaponBox;

	UPROPERTY(VisibleAnywhere)
	class UWeaponComponent* WeaponComponent;

	UPROPERTY(VisibleAnywhere)
	class USoundComponent* SoundComponent;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	EWeaponType WeaponType = EWeaponType::EWT_OneHandedSword;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	FName WeaponHolsterSocketName = "HipWeaponHolsterSocket";

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	FName WeaponHandSocketName = "One-HandedWeaponSocket";


};
