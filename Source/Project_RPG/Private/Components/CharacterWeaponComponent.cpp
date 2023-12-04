// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CharacterWeaponComponent.h"
#include "GameFramework/Character.h"
#include "Characters/PlayerCharacter.h"
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

	Character = Cast<ACharacter>(GetOwner()->GetOwner());
}


// Called every frame
void UCharacterWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCharacterWeaponComponent::SetOwnerAsPlayer()
{
	if (!Character)
	{
		Character = Cast<ACharacter>(GetOwner()->GetOwner());
	}
}

void UCharacterWeaponComponent::PlayDrawWeaponMontage()
{
	if (!bDrawWeapon)
	{
		if (Character)
		{
			UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
			if (AnimInstance && DrawWeaponMontage)
			{
				APlayerCharacter* PlayerCharacter = IReferencesInterface::Execute_RI_GetPlayerCharacter(Character);
				if (PlayerCharacter->GetActionState() == EActionState::EAS_Unoccupied)
				{
					AnimInstance->Montage_Play(DrawWeaponMontage);
					bDrawWeapon = true;
					bHolsterWeapon = false;
				}
			}
		}
	}
}

void UCharacterWeaponComponent::PlayHolsterWeaponMontage()
{
	if (!bHolsterWeapon)
	{
		if (Character)
		{
			UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
			if (AnimInstance && HolsterWeaponMontage)
			{
				APlayerCharacter* PlayerCharacter = IReferencesInterface::Execute_RI_GetPlayerCharacter(Character);
				if (PlayerCharacter->GetActionState() == EActionState::EAS_Unoccupied)
				{
					AnimInstance->Montage_Play(HolsterWeaponMontage);
					bDrawWeapon = false;
					bHolsterWeapon = true;
				}
			}
		}
	}
}

void UCharacterWeaponComponent::PlayAttackMontage()
{
	if (Character)
	{
		UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
		if (AnimInstance && AttackMontage)
		{
			APlayerCharacter* PlayerCharacter = IReferencesInterface::Execute_RI_GetPlayerCharacter(Character);
			if (PlayerCharacter->GetActionState() == EActionState::EAS_Unoccupied)
			{
				AnimInstance->Montage_Play(AttackMontage);
				bDrawWeapon = true;
				bHolsterWeapon = false;
			}
		}
	}
}
