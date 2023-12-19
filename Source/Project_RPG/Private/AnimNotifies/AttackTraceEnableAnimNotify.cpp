// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/AttackTraceEnableAnimNotify.h"
/* Characters */
#include "Characters/PlayerCharacterAnimInstance.h"
#include "Characters/PlayerCharacter.h"
/* Items */
#include "Items/Weapons/Weapon.h"
/* Components */
#include "Components/WeaponComponent.h"

void UAttackTraceEnableAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	UPlayerCharacterAnimInstance* AnimInstance = Cast<UPlayerCharacterAnimInstance>(MeshComp->GetAnimInstance()); //IReferencesInterface::Execute_RI_GetPlayerAnimInstance(MeshComp->GetAnimInstance());
	if (AnimInstance)
	{
		AWeapon* Weapon = AnimInstance->PlayerCharacter->GetGrabbedWeapon();
		if (Weapon)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, FString::Printf(TEXT("Attack Trace Enable Anim Notify")));
			Weapon->SetWeaponBoxCollisionEnabled();
			Weapon->GetWeaponComponent()->SpawnWeaponTrailEffect();
		}
	}
}
