// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"
/* Components */
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/CharacterWeaponComponent.h"
#include "Components/ChildActorComponent.h"
/* Kismet */
#include "Kismet/KismetSystemLibrary.h"
/* Weapon */
#include "Items/Weapons/Weapon.h"


// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// Pure C++ Interface Usage
//Called When a Weapon Hits this character, this used for playing correct HitMontages when this character gets hit.
void AEnemy::WI_GetWeaponHit(const FVector& ImpactPoint)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("GetWeaponHit")));
	DrawDebugSphere(GetWorld(), ImpactPoint, 10.f, 12, FColor::Red, false, 5.0f);
	//Weapon->GetCharacterWeaponComponent()->PlayHitReactionMontage("HitFromFront");

	const FVector Forward = GetActorForwardVector();
	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	/* GetSafeNormal is going to take the vector, which is the result of this subtraction and normalize it and then return that result which we're storing in ToHit. 
	So now to hit is normalized. GetSafeNormal means that it's going to check to make sure that it's safe to normalize the vector. */
	const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();

	// Foward * ToHit = |Forward||ToHit| * cos(theta)
	// |Forward| = 1, |ToHit| = 1, so Forward * ToHit = cos(theta)
	const double CosTheta = FVector::DotProduct(Forward, ToHit);
	// Take the inverse cosine (arc-cosine) of cos(theta) to get theta
	double Theta = FMath::Acos(CosTheta);
	// Convert from radians to degrees
	Theta = FMath::RadiansToDegrees(Theta);

	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("Theta: %f "), Theta));
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + Forward * 60.0f, 5.f, FColor::Red, 5.f);
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + ToHit * 60.0f, 5.f, FColor::Green, 5.f);
}

