// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CharacterWeaponComponent.h"
#include "GameFramework/Character.h"
/* Animation */
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
/* Items */
#include "Items/Weapons/Weapon.h"

// Sets default values for this component's properties
UCharacterWeaponComponent::UCharacterWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCharacterWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<ACharacter>(GetOwner());
}


// Called every frame
void UCharacterWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCharacterWeaponComponent::PlayDrawWeaponMontage(UAnimMontage* WeaponDrawMontage)
{
	if (!bDrawWeapon)
	{
		if (Character)
		{
			if (WeaponDrawMontage)
			{
				UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
				if (AnimInstance)
				{
					AnimInstance->Montage_Play(WeaponDrawMontage);
					bDrawWeapon = true;
					bHolsterWeapon = false;
				}
			}
		}
	}
}

void UCharacterWeaponComponent::PlayHolsterWeaponMontage(UAnimMontage* WeaponHolsterMontage)
{
	if (!bHolsterWeapon)
	{
		if (Character)
		{
			if (WeaponHolsterMontage)
			{
				UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
				if (AnimInstance)
				{
					AnimInstance->Montage_Play(WeaponHolsterMontage);
					bDrawWeapon = false;
					bHolsterWeapon = true;
				}
			}
		}
	}
}

void UCharacterWeaponComponent::PlayAttackMontage(UAnimMontage* AttackMontage)
{
	if (Character)
	{
		if (AttackMontage)
		{
			UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
			if (AnimInstance)
			{
				AnimInstance->Montage_Play(AttackMontage);
				bDrawWeapon = true;
				bHolsterWeapon = false;
			}
		}
	}
}
