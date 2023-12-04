// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/UnoccupiedAnimNoify.h"
/* Characters */
#include "Characters/PlayerCharacterAnimInstance.h"
#include "Characters/PlayerCharacter.h"

void UUnoccupiedAnimNoify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	UPlayerCharacterAnimInstance* AnimInstance = Cast<UPlayerCharacterAnimInstance>(MeshComp->GetAnimInstance()); //IReferencesInterface::Execute_RI_GetPlayerAnimInstance(MeshComp->GetAnimInstance());
	if (AnimInstance)
	{
		AnimInstance->PlayerCharacter->SetActionState(EActionState::EAS_Unoccupied);
	}
}
