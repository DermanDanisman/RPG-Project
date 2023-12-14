// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"
/* Components */
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/CharacterWeaponComponent.h"
#include "Components/ChildActorComponent.h"
#include "Components/CharacterMontageComponent.h"
#include "Components/SoundComponent.h"
/* Kismet */
#include "Kismet/KismetSystemLibrary.h"
/* Weapon */
#include "Items/Weapons/Weapon.h"


// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CharacterMontageComponent = CreateDefaultSubobject<UCharacterMontageComponent>(TEXT("CharacterMontageComponent"));
	CharacterMontageComponent->SetComponentTickEnabled(false);

	SoundComponent = CreateDefaultSubobject<USoundComponent>(TEXT("SoundComponent"));

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

	CharacterMontageComponent->PlayHitReactionMontage(ImpactPoint);
	SoundComponent->PlayHitSound(ImpactPoint);
}

