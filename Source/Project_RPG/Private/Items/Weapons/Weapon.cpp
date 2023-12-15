// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Weapon.h"
#include "Characters/PlayerCharacter.h"
/* Components */
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WeaponComponent.h"
#include "Components/SoundComponent.h"
/* Kismet */
#include "Kismet/KismetSystemLibrary.h"


AWeapon::AWeapon()
{
	WeaponComponent = CreateDefaultSubobject<UWeaponComponent>(TEXT("WeaponComponent"));
	SoundComponent = CreateDefaultSubobject<USoundComponent>(TEXT("SoundComponent"));

	WeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponBox"));
	WeaponBox->SetupAttachment(ItemMesh, "WeaponBoxCollision");
	WeaponBox->SetGenerateOverlapEvents(true);
	WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	WeaponBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	WeaponBox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnWeaponBoxOverlap);
}

void AWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnSphereEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

// Weapon Box Collision Overlap Function Triggers Box Trace For Hits
void AWeapon::OnWeaponBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	WeaponComponent->BoxTrace();
}

// Enabling Weapon Box Collision
void AWeapon::SetWeaponBoxCollisionEnabled()
{
	WeaponBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

// Disabling Weapon Box Collision
void AWeapon::SetWeaponBoxCollisionDisabled()
{
	WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponComponent->ClearIgnoreActors();
}

// Attaching Mesh To Proper Socket
void AWeapon::AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName)
{
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	ItemMesh->AttachToComponent(InParent, AttachmentRules, InSocketName);
}

// Called When Player Presses 'E' Key to Pickup Weapon
void AWeapon::PickupWeapon(USceneComponent* InParent, FName InSocketName)
{
	SimulatePhysics(false);
	PickupSphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttachMeshToSocket(InParent, InSocketName);
}

// Drawing Weapon Attaches Weapon To Hand
void AWeapon::DrawWeapon(USceneComponent* InParent, FName InSocketName)
{
	AttachMeshToSocket(InParent, InSocketName);
}

// Holstering Weapon Attaches Weapon To Proper Socket
void AWeapon::HolsterWeapon(USceneComponent* InParent, FName InSocketName)
{
	AttachMeshToSocket(InParent, InSocketName);
}


