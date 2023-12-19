// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/WeaponComponent.h"
#include "GameFramework/Character.h"
/* Animation */
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
/* Components */
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
/* Kismet */
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
/* Interfaces */
#include "Interfaces/WeaponInterface.h"
/* Particles */
#include "Particles/ParticleSystemComponent.h"


// Sets default values for this component's properties
UWeaponComponent::UWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner()->GetOwner());
	OwnerActor = Cast<AActor>(GetOwner());
	OwnerStaticMesh = Cast<UStaticMeshComponent>(GetOwner()->GetComponentByClass(UStaticMeshComponent::StaticClass()));

	TraceObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery1); // World Static
	TraceObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery2); // World Dynamic
	TraceObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery3); // Pawn
	//TraceObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery4); // Physics Body
	//TraceObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery5); // Vehicle
	TraceObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery6); // Destructible
}

	
// Called every frame
void UWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

/* Sets the OwnerCharacter reference to the owner of this component if it's a character. 
This is used to ensure that the component has a reference to the character that is using the weapon. */
void UWeaponComponent::SetOwnerAsPlayer()
{
	if (!OwnerCharacter)
	{
		OwnerCharacter = Cast<ACharacter>(GetOwner()->GetOwner());
	}
}

/* Plays an animation montage from a specified section. It checks if the owning character and the montage are valid before playing. 
This is used to play specific parts of an animation, like an attack or reload sequence. */
void UWeaponComponent::PlayMontageFromSection(UAnimMontage* Montage, const FName& SectionName)
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

/* Plays the drawing weapon animation montage. It sets bDrawWeapon to true, indicating the weapon is drawn. 
This function is called when the character equips the weapon. */
void UWeaponComponent::PlayDrawWeaponMontage()
{
	if (OwnerCharacter)
	{
		UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
		if (AnimInstance && WeaponStandardMontages.DrawWeaponMontage)
		{
			PlayMontageFromSection(WeaponStandardMontages.DrawWeaponMontage);
			bDrawWeapon = true;
		}
	}
}

/* Similar to PlayDrawWeaponMontage, but for holstering the weapon. 
It sets bDrawWeapon to false, indicating the weapon is holstered. */
void UWeaponComponent::PlayHolsterWeaponMontage()
{
	if (OwnerCharacter)
	{
		UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
		if (AnimInstance && WeaponStandardMontages.HolsterWeaponMontage)
		{
			PlayMontageFromSection(WeaponStandardMontages.HolsterWeaponMontage);
			bDrawWeapon = false;
		}
	}
}

/* Plays the attack animation montage. 
This function is called when the player initiates an attack with the weapon. */
void UWeaponComponent::PlayAttackMontage()
{
	if (OwnerCharacter)
	{
		UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
		if (AnimInstance && AttackMontage)
		{
			PlayMontageFromSection(AttackMontage);
			bDrawWeapon = true;
		}
	}
}

void UWeaponComponent::PlayDodgeMontage()
{
	if (OwnerCharacter)
	{
		UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
		if (AnimInstance && WeaponStandardMontages.DodgeMontage)
		{
			// Get the last movement input vector and normalize it
			FVector LastInputVector = OwnerCharacter->GetLastMovementInputVector().GetSafeNormal();

			// Get the character's forward and right vectors for reference
			FVector ForwardVector = OwnerCharacter->GetActorForwardVector();
			FVector RightVector = OwnerCharacter->GetActorRightVector();

			// Determine the dodge direction based on the dot product
			// Dot product will give a value between -1 and 1 indicating alignment
			float ForwardDot = FVector::DotProduct(LastInputVector, ForwardVector);
			float RightDot = FVector::DotProduct(LastInputVector, RightVector);

			FName SectionName; // Variable to hold the section name of the montage
			// Determine the dodge direction and set the montage section name accordingly
			if (ForwardDot > 0.707f) // Forward dodge if the input is mostly forward
				SectionName = FName(WeaponStandardMontages.DodgeMontage->GetSectionName(1));
			else if (ForwardDot < -0.707f) // Backward dodge if the input is mostly backward
				SectionName = FName(WeaponStandardMontages.DodgeMontage->GetSectionName(0));
			else if (RightDot > 0.707f) // Right dodge if the input is mostly to the right
				SectionName = FName(WeaponStandardMontages.DodgeMontage->GetSectionName(3));
			else if (RightDot < -0.707f) // Left dodge if the input is mostly to the left
				SectionName = FName(WeaponStandardMontages.DodgeMontage->GetSectionName(2));
			else
				SectionName = FName(WeaponStandardMontages.DodgeMontage->GetSectionName(0)); // Default to backward if the direction is unclear

			// Play the montage section based on the determined direction
			PlayMontageFromSection(WeaponStandardMontages.DodgeMontage, SectionName);
		}
	}
}

/* Performs a box trace (collision check) from the weapon to detect hits. 
It uses the start and end points defined by sockets on the weapon's mesh. 
The function filters out the owner and the owning character from the hit detection to prevent the weapon from hitting itself or the character wielding it. 
If the hit actor implements the UWeaponInterface, it calls the WI_GetWeaponHit() function on that actor. 
This is used for detecting when the weapon hits an enemy or object. */
FHitResult UWeaponComponent::BoxTrace()
{
	FVector Start;
	FVector End;

	TArray<AActor*> ActorsToIgnore;

	FHitResult HitResult;

	if (OwnerCharacter)
	{
		if (OwnerStaticMesh)
		{
			if (OwnerStaticMesh->DoesSocketExist(TraceStartSocketName) && OwnerStaticMesh->DoesSocketExist(TraceEndSocketName))
			{
				Start = OwnerStaticMesh->GetSocketLocation(TraceStartSocketName);
				End = OwnerStaticMesh->GetSocketLocation(TraceEndSocketName);
			}
		}

		IgnoreActors.Add(GetOwner());
		IgnoreActors.Add(OwnerCharacter);

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
			OwnerStaticMesh->GetComponentRotation(),
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
			EDrawDebugTrace::None,
			HitResult,
			true,
			FLinearColor::Red,
			FLinearColor::Green,
			5.0f
		);

		if (HitResult.GetActor() && HitResult.GetActor()->Implements<UWeaponInterface>())
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("Hit Actor Name: %s"), *HitResult.GetActor()->GetName()));
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

/* Creates and activates a weapon trail particle effect. 
This effect is used to visually indicate the weapon's movement, especially during attacks. 
The trails are started between two specified socket locations on the weapon. */
void UWeaponComponent::SpawnWeaponTrailEffect()
{
	// Check if the particle system is already created
	if (!WeaponTrailEffectSystem && OwnerStaticMesh && WeaponTrailEffect)
	{
		// Spawn and attach the particle system if not already created
		WeaponTrailEffectSystem = UGameplayStatics::SpawnEmitterAttached(WeaponTrailEffect, OwnerStaticMesh);
	}

	if (WeaponTrailEffectSystem)
	{
		// Begin the weapon trail effect
		WeaponTrailEffectSystem->SetHiddenInGame(false);
		WeaponTrailEffectSystem->ActivateSystem();
		WeaponTrailEffectSystem->BeginTrails("TraceStart", "TraceEnd", ETrailWidthMode_FromCentre, 1.0f);
	}
}

/* Hides the weapon trail effect, effectively turning it off. 
This is called when the weapon is not in use or after an attack is completed. */
void UWeaponComponent::DespawnWeaponTrailEffect()
{
	if (WeaponTrailEffectSystem)
	{
		WeaponTrailEffectSystem->SetHiddenInGame(true);
	}
}

