// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Enums/WeaponType.h"
#include "Weapon.generated.h"

/**
 * 
 */

UCLASS()
class PROJECT_RPG_API AWeapon : public AItem
{
	GENERATED_BODY()

public:

	AWeapon();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/// <summary>
	/// Getter Functions

	UFUNCTION()
	FORCEINLINE UWeaponComponent* GetWeaponComponent() const { return WeaponComponent; }

	UFUNCTION()
	FORCEINLINE EWeaponType GetWeaponType() const;

	UFUNCTION()
	FORCEINLINE FName GetWeaponHolsterSocketName() const;

	UFUNCTION()
	FORCEINLINE FName GetWeaponHandSocketName() const;

	/// Getter Functions
	/// </summary>

public:

	UFUNCTION()
	void SetWeaponBoxCollisionEnabled();

	UFUNCTION()
	void SetWeaponBoxCollisionDisabled();

	UFUNCTION()
	void AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName);

	UFUNCTION()
	void PickupWeapon(USceneComponent* InParent, FName InSocketName);

	UFUNCTION(BlueprintCallable)
	void DrawWeapon(USceneComponent* InParent, FName InSocketName);

	UFUNCTION(BlueprintCallable)
	void HolsterWeapon(USceneComponent* InParent, FName InSocketName);

	UFUNCTION()
	void Dodge();


	
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
};
