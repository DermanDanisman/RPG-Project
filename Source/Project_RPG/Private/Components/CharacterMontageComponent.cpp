// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CharacterMontageComponent.h"
#include "GameFramework/Character.h"
/* Kismet */
#include "Kismet/KismetSystemLibrary.h"

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

	OwnerCharacter = Cast<ACharacter>(GetOwner());

}


// Called every frame
void UCharacterMontageComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCharacterMontageComponent::PlayMontageFromSection(UAnimMontage* Montage, const FName& SectionName)
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

void UCharacterMontageComponent::PlayHitReactionMontage(const FVector& ImpactPoint)
{
	if (OwnerCharacter)
	{
		UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
		if (AnimInstance && HitReactionMontage)
		{
			/* Direction Calculation */

			const FVector Forward = OwnerCharacter->GetActorForwardVector();
			const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, OwnerCharacter->GetActorLocation().Z);
			/* GetSafeNormal is going to take the vector, which is the result of this subtraction and normalize it and then return that result which we're storing in ToHit.
			So now to hit is normalized. GetSafeNormal means that it's going to check to make sure that it's safe to normalize the vector. */
			const FVector ToHit = (ImpactLowered - OwnerCharacter->GetActorLocation()).GetSafeNormal();

			// Foward * ToHit = |Forward||ToHit| * cos(theta)
			// |Forward| = 1, |ToHit| = 1, so Forward * ToHit = cos(theta)
			const double CosTheta = FVector::DotProduct(Forward, ToHit);
			// Take the inverse cosine (arc-cosine) of cos(theta) to get theta
			double Theta = FMath::Acos(CosTheta);
			// Convert from radians to degrees
			Theta = FMath::RadiansToDegrees(Theta);

			// If CrossProduct points down, Theta should be negative
			const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
			if (CrossProduct.Z < 0)
			{
				Theta *= -1.f;
			}

			// Section Index 1 is HitFromBack
			FName Section(HitReactionMontage->GetSectionName(1));
			if (Theta >= -45 && Theta < 45.f)// Section Index 0 is HitFromFront
				Section = FName(HitReactionMontage->GetSectionName(0));
			else if (Theta >= -135.f && Theta < -45.f)// Section Index 0 is HitFromLeft
				Section = FName(HitReactionMontage->GetSectionName(2));
			else if (Theta >= 45.f && Theta < 135.f)// Section Index 0 is HitFromRight
				Section = FName(HitReactionMontage->GetSectionName(3));

			UKismetSystemLibrary::DrawDebugArrow(this, OwnerCharacter->GetActorLocation(), OwnerCharacter->GetActorLocation() + CrossProduct * 100.0f, 5.f, FColor::Blue, 5.f);
			UKismetSystemLibrary::DrawDebugArrow(this, OwnerCharacter->GetActorLocation(), OwnerCharacter->GetActorLocation() + Forward * 60.0f, 5.f, FColor::Red, 5.f);
			UKismetSystemLibrary::DrawDebugArrow(this, OwnerCharacter->GetActorLocation(), OwnerCharacter->GetActorLocation() + ToHit * 60.0f, 5.f, FColor::Green, 5.f);
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("Theta: %f "), Theta));

			PlayMontageFromSection(HitReactionMontage, Section);
		}
	}
}

/*void UCharacterMontageComponent::PlayDodgeMontage()
{
	// Check if DodgeMontage is set and OwnerCharacter is valid
	if (DodgeMontage && OwnerCharacter)
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
			SectionName = FName(DodgeMontage->GetSectionName(1));
		else if (ForwardDot < -0.707f) // Backward dodge if the input is mostly backward
			SectionName = FName(DodgeMontage->GetSectionName(0));
		else if (RightDot > 0.707f) // Right dodge if the input is mostly to the right
			SectionName = FName(DodgeMontage->GetSectionName(3));
		else if (RightDot < -0.707f) // Left dodge if the input is mostly to the left
			SectionName = FName(DodgeMontage->GetSectionName(2));
		else
			SectionName = FName(DodgeMontage->GetSectionName(0)); // Default to backward if the direction is unclear

		// Play the montage section based on the determined direction
		PlayMontageFromSection(DodgeMontage, SectionName);
	}
}*/

