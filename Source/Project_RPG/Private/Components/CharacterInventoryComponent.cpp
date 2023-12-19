// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CharacterInventoryComponent.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
UCharacterInventoryComponent::UCharacterInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UCharacterInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
}

// Called every frame
void UCharacterInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCharacterInventoryComponent::AddItemToInventory(AActor* Item)
{
	if (Item->GetClass()->IsChildOf(WeaponClass))
	{
		WeaponList.AddUnique(Item);
		for (AActor* Weapon : WeaponList)
		{
			if (Weapon)
			{
				// Set the visibility based on whether this is the currently equipped weapon
				Weapon->SetActorHiddenInGame(Weapon != Item);
			}
		}
	}
}

void UCharacterInventoryComponent::RemoveItemFromInventory(AActor* Item)
{
	if (Item->GetClass()->IsChildOf(WeaponClass))
	{
		int ItemIndex = WeaponList.Find(Item);
		WeaponList.RemoveAt(ItemIndex);
	}
}


