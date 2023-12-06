// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Weapon.h"
#include "Characters/PlayerCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/CharacterWeaponComponent.h"
/* Components */
#include "Components/SkeletalMeshComponent.h"


AWeapon::AWeapon()
{
	CharacterWeaponComponent = CreateDefaultSubobject<UCharacterWeaponComponent>(TEXT("CharacterWeaponComponent"));

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(GetRootComponent());
}

void AWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

}

void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnSphereEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

}

void AWeapon::AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName)
{
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	WeaponMesh->AttachToComponent(InParent, AttachmentRules, InSocketName);
}

void AWeapon::EquipWeapon(USceneComponent* InParent, FName InSocketName)
{
	SimulatePhysics(false);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttachMeshToSocket(InParent, InSocketName);
}

void AWeapon::DrawWeapon(USceneComponent* InParent, FName InSocketName)
{
	AttachMeshToSocket(InParent, InSocketName);
}

void AWeapon::HolsterWeapon(USceneComponent* InParent, FName InSocketName)
{
	AttachMeshToSocket(InParent, InSocketName);
}



