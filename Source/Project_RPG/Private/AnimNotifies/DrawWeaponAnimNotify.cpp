// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/DrawWeaponAnimNotify.h"
/* Characters */
#include "Characters/PlayerCharacterAnimInstance.h"
#include "Characters/PlayerCharacter.h"
/* Items */
#include "Items/Weapons/Weapon.h"

void UDrawWeaponAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	UPlayerCharacterAnimInstance* AnimInstance = Cast<UPlayerCharacterAnimInstance>(MeshComp->GetAnimInstance()); //IReferencesInterface::Execute_RI_GetPlayerAnimInstance(MeshComp->GetAnimInstance());
	if (AnimInstance)
	{
		AWeapon* Weapon = AnimInstance->PlayerCharacter->GetGrabbedWeapon();
		if (Weapon)
		{
			if (Weapon->GetWeaponType() == EWeaponType::EWT_OneHandedSword)
			{
				Weapon->DrawWeapon(AnimInstance->PlayerCharacter->GetMesh(), Weapon->GetWeaponHandSocketName());
				AnimInstance->PlayerCharacter->SetCharacterState(ECharacterState::ECS_OneHandedSword);
				//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, FString::Printf(TEXT("EWT_OneHandedSword")));
			}
			else if (Weapon->GetWeaponType() == EWeaponType::EWT_TwoHandedSword)
			{
				Weapon->DrawWeapon(AnimInstance->PlayerCharacter->GetMesh(), Weapon->GetWeaponHandSocketName());
				AnimInstance->PlayerCharacter->SetCharacterState(ECharacterState::ECS_TwoHandedSword);
				//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, FString::Printf(TEXT("EWT_TwoHandedSword")));
			}
		}
	}
}
