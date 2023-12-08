// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Weapon.h"
#include "Characters/PlayerCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/CharacterWeaponComponent.h"
/* Components */
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
/* Kismet */
#include "Kismet/KismetSystemLibrary.h"


AWeapon::AWeapon()
{
	CharacterWeaponComponent = CreateDefaultSubobject<UCharacterWeaponComponent>(TEXT("CharacterWeaponComponent"));

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(GetRootComponent());
	WeaponMesh->SetCollisionProfileName(FName("Item"));
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh->BodyInstance.bNotifyRigidBodyCollision = true;
	WeaponMesh->SetGenerateOverlapEvents(true);
	WeaponMesh->SetReceivesDecals(false);

	/*WeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponBox"));
	WeaponBox->SetupAttachment(WeaponMesh);
	WeaponBox->SetGenerateOverlapEvents(true);
	WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	WeaponBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);*/
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//CharacterWeaponComponent->BoxTrace();
}


void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	WeaponMesh->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnWeaponMeshOverlap);
	//WeaponBox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnWeaponBoxOverlap);
}



void AWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

}

void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnSphereEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

}

void AWeapon::OnWeaponMeshOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != this && OtherActor != GetOwner())
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("Hit Actor Name: %s"), *OtherActor->GetName()));
	
	}
	CharacterWeaponComponent->BoxTrace();
}

void AWeapon::OnWeaponBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}

void AWeapon::SetWeaponBoxCollisionEnabled()
{
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void AWeapon::SetWeaponBoxCollisionDisabled()
{
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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


