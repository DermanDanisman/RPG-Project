// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterInventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_RPG_API UCharacterInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterInventoryComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	FORCEINLINE AActor* GetLastPickedUpWeapon() { return WeaponList.Last(); }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:

	UFUNCTION()
	void AddItemToInventory(AActor* Item);

	UFUNCTION()
	void RemoveItemFromInventory(AActor* Item);

public:	

	/* References */
	UPROPERTY(VisibleAnywhere, Category = "References")
	class ACharacter* OwnerCharacter = nullptr;

private:

	UPROPERTY(EditAnywhere, Category = "Item Class")
	TSubclassOf<AActor> WeaponClass;

	UPROPERTY(VisibleAnywhere, Category = "Weapon List")
	TArray<AActor*> WeaponList;

		
};
