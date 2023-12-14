// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CharacterWeaponComponent.h"
#include "GameFramework/Character.h"
#include "Characters/PlayerCharacter.h"
/* Animation */
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
/* Items */
#include "Items/Weapons/Weapon.h"
/* Components */
#include "Components/SkeletalMeshComponent.h"
/* Kismet */
#include "Kismet/KismetSystemLibrary.h"
/* Interfaces */
#include "Interfaces/WeaponInterface.h"

// Sets default values for this component's properties
UCharacterWeaponComponent::UCharacterWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UCharacterWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner()->GetOwner());
	//SkeletalMesh = Cast<USkeletalMeshComponent>(GetOwner()->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
	OwnerStaticMesh = Cast<UStaticMeshComponent>(GetOwner()->GetComponentByClass(UStaticMeshComponent::StaticClass()));

	TraceObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery1);
	TraceObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery2);
	TraceObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery3);
	TraceObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery4);
	TraceObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery5);
	TraceObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery6);
}


// Called every frame
void UCharacterWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCharacterWeaponComponent::SetOwnerAsPlayer()
{
	if (!OwnerCharacter)
	{
		OwnerCharacter = Cast<ACharacter>(GetOwner()->GetOwner());
	}
}

void UCharacterWeaponComponent::PlayMontageFromSection(UAnimMontage* Montage, const FName& SectionName)
{
	if (OwnerCharacter)
	{
		UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
		if (AnimInstance && Montage)
		{
			AnimInstance->Montage_Play(Montage);
			AnimInstance->Montage_JumpToSection(SectionName, Montage);
		}
	}
}

void UCharacterWeaponComponent::PlayDrawWeaponMontage()
{
	if (OwnerCharacter)
	{
		UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
		if (AnimInstance && DrawWeaponMontage)
		{
			PlayMontageFromSection(DrawWeaponMontage);
			bDrawWeapon = true;
			bHolsterWeapon = false;
		}
	}
}

void UCharacterWeaponComponent::PlayHolsterWeaponMontage()
{
	if (OwnerCharacter)
	{
		UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
		if (AnimInstance && HolsterWeaponMontage)
		{
			PlayMontageFromSection(HolsterWeaponMontage);
			bDrawWeapon = false;
			bHolsterWeapon = true;
		}
	}
}

void UCharacterWeaponComponent::PlayAttackMontage()
{
	if (OwnerCharacter)
	{
		UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
		if (AnimInstance && AttackMontage)
		{
			PlayMontageFromSection(AttackMontage);
			bDrawWeapon = true;
			bHolsterWeapon = false;
		}
	}
}

FHitResult UCharacterWeaponComponent::BoxTrace()
{
	FVector Start;
	FVector End;

	TArray<AActor*> ActorsToIgnore;

	FHitResult HitResult;

	if (OwnerCharacter)
	{
		ActorsToIgnore.Add(GetOwner());
		ActorsToIgnore.Add(OwnerCharacter);

		if (OwnerStaticMesh)
		{
			if (OwnerStaticMesh->DoesSocketExist(TraceStartSocketName) && OwnerStaticMesh->DoesSocketExist(TraceEndSocketName))
			{
				Start = OwnerStaticMesh->GetSocketLocation(TraceStartSocketName);
				End = OwnerStaticMesh->GetSocketLocation(TraceEndSocketName);
			}
		}

		if (!IgnoreActors.IsEmpty())
		{
			for (AActor* Actor : IgnoreActors)
			{
				ActorsToIgnore.AddUnique(Actor);
			}
		}

		/*UKismetSystemLibrary::BoxTraceSingle(
			this,
			Start,
			End,
			FVector(3.f, 3.f, 3.f),
			SkeletalMesh->GetComponentRotation(),
			ETraceTypeQuery::TraceTypeQuery1,
			false,
			ActorsToIgnore,
			EDrawDebugTrace::ForDuration,
			HitResult,
			true,
			FLinearColor::Red,
			FLinearColor::Green,
			5.0f
		);*/

		UKismetSystemLibrary::BoxTraceSingleForObjects(
			this,
			Start,
			End,
			FVector(3.0f, 3.0f, 3.0f),
			OwnerStaticMesh->GetComponentRotation(),
			TraceObjectTypes,
			false,
			ActorsToIgnore,
			EDrawDebugTrace::ForDuration,
			HitResult,
			true,
			FLinearColor::Red,
			FLinearColor::Green,
			5.0f
		);

		if (HitResult.GetActor() && HitResult.GetActor()->Implements<UWeaponInterface>())
		{
			//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("Hit Actor Name: %s"), *HitResult.GetActor()->GetName()));
			// Pure C++ Interface Usage
			if (HitResult.GetActor()->Implements<UWeaponInterface>())
			{
				IWeaponInterface* WeaponInterface = Cast<IWeaponInterface>(HitResult.GetActor());
				if (WeaponInterface)
				{
					WeaponInterface->WI_GetWeaponHit(HitResult.ImpactPoint);
				}
				// This is for player to stop hitting multiple times to the same actor.
				IgnoreActors.AddUnique(HitResult.GetActor());
			}
		}
		return HitResult;
	}
	return HitResult;
}


