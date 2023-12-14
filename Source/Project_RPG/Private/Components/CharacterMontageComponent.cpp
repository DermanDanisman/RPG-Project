// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CharacterMontageComponent.h"
#include "Characters/PlayerCharacter.h"
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

			if (Theta >= -45 && Theta < 45.f)
			{
				// Section Index 0 is HitFromFront
				Section = FName(HitReactionMontage->GetSectionName(0));
			}
			else if (Theta >= -135.f && Theta < -45.f)
			{
				// Section Index 0 is HitFromLeft
				Section = FName(HitReactionMontage->GetSectionName(2));
			}
			else if (Theta >= 45.f && Theta < 135.f)
			{
				// Section Index 0 is HitFromRight
				Section = FName(HitReactionMontage->GetSectionName(3));
			}

			UKismetSystemLibrary::DrawDebugArrow(this, OwnerCharacter->GetActorLocation(), OwnerCharacter->GetActorLocation() + CrossProduct * 100.0f, 5.f, FColor::Blue, 5.f);

			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("Theta: %f "), Theta));
			UKismetSystemLibrary::DrawDebugArrow(this, OwnerCharacter->GetActorLocation(), OwnerCharacter->GetActorLocation() + Forward * 60.0f, 5.f, FColor::Red, 5.f);
			UKismetSystemLibrary::DrawDebugArrow(this, OwnerCharacter->GetActorLocation(), OwnerCharacter->GetActorLocation() + ToHit * 60.0f, 5.f, FColor::Green, 5.f);

			PlayMontageFromSection(HitReactionMontage, Section);
		}
	}
}

