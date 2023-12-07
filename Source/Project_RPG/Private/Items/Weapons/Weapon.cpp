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
	WeaponMesh->BodyInstance.bNotifyRigidBodyCollision = true;
	WeaponMesh->SetGenerateOverlapEvents(true);
	WeaponMesh->SetReceivesDecals(false);

	/*WeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponBox"));
	WeaponBox->SetupAttachment(WeaponMesh);
	WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);*/

	TraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("TraceStart"));
	TraceStart->SetupAttachment(WeaponMesh);

	TraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("TraceEnd"));
	TraceEnd->SetupAttachment(WeaponMesh);
}


void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	WeaponMesh->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnWeaponMeshOverlap);
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
	const FVector Start = TraceStart->GetComponentLocation();
	const FVector End = TraceEnd->GetComponentLocation();

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	ActorsToIgnore.Add(GetOwner());

	FHitResult HitResult;

	UKismetSystemLibrary::BoxTraceSingle(
		this,
		Start,
		End,
		FVector(3.f, 3.f, 3.f),
		TraceStart->GetComponentRotation(),
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		HitResult,
		true,
		FLinearColor::Red,
		FLinearColor::Green,
		5.0f
	);
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


