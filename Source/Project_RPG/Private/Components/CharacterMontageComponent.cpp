// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CharacterMontageComponent.h"
#include "Characters/PlayerCharacter.h"

// Sets default values for this component's properties
UCharacterMontageComponent::UCharacterMontageComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UCharacterMontageComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	if (!OwnerCharacter)
	{
		OwnerCharacter = Cast<ACharacter>(GetOwner()->GetOwner());
	}
}


// Called every frame
void UCharacterMontageComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCharacterMontageComponent::PlayMontageFromSection(UAnimMontage* Montage, const FName& SectionName)
{
	UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
	if (AnimInstance && Montage)
	{
		AnimInstance->Montage_Play(Montage);
		AnimInstance->Montage_JumpToSection(SectionName, Montage);
	}
}

void UCharacterMontageComponent::PlayHitReactionMontage(const FName& SectionName)
{
	if (OwnerCharacter)
	{
		UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
		if (AnimInstance && HitReactionMontage)
		{
			if (SectionName != "") PlayMontageFromSection(HitReactionMontage, SectionName);
			else PlayMontageFromSection(HitReactionMontage);
		}
	}
}

