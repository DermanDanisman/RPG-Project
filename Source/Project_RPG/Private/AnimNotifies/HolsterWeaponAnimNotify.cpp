// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/HolsterWeaponAnimNotify.h"
/* Characters */
#include "Characters/PlayerCharacterAnimInstance.h"
#include "Characters/PlayerCharacter.h"
/* Items */
#include "Items/Weapons/Weapon.h"

void UHolsterWeaponAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	UPlayerCharacterAnimInstance* AnimInstance = Cast<UPlayerCharacterAnimInstance>(MeshComp->GetAnimInstance());
	if (AnimInstance)
	{
		AWeapon* Weapon = AnimInstance->PlayerCharacter->GetGrabbedWeapon();
		if (Weapon)
		{
			if (Weapon->GetWeaponType() == EWeaponType::EWT_OneHandedSword)
			{
				Weapon->HolsterWeapon(AnimInstance->PlayerCharacter->GetMesh(), FName("HipWeaponHolsterSocket"));
				AnimInstance->PlayerCharacter->SetCharacterState(ECharacterState::ECS_Unequipped);
			}
			else if (Weapon->GetWeaponType() == EWeaponType::EWT_TwoHandedSword)
			{
				Weapon->HolsterWeapon(AnimInstance->PlayerCharacter->GetMesh(), FName("BackWeaponHolsterLocation"));
				AnimInstance->PlayerCharacter->SetCharacterState(ECharacterState::ECS_Unequipped);
			}
		}
	}
}
