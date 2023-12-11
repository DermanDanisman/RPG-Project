// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"
/* Components */
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
/* Player Character */
#include "Characters/PlayerCharacter.h"
/* Interfaces */
#include "Interfaces/PlayerInputInterface.h"
#include "Interfaces/ReferencesInterface.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRootComponent"));
	SetRootComponent(DefaultRootComponent);

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	ItemMesh->SetupAttachment(GetRootComponent());
	ItemMesh->SetCollisionProfileName(FName("Item"));

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlap);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//RunningTime += DeltaTime;

	//AddActorWorldOffset(FVector(0.f, 0.f, TransformedSin()));
}


void AItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		// Check if OtherActor implements IPlayerInputInterface
		if (OtherActor->GetClass()->ImplementsInterface(UPlayerInputInterface::StaticClass()))
		{
			if (ItemMappingContext)
			{
				// Adding Input Mapping Context for item to be picked up when overlapped
				IPlayerInputInterface* PlayerInputInterface = Cast<IPlayerInputInterface>(OtherActor);
				if (PlayerInputInterface)
				{
					PlayerInputInterface->PII_AddInputMappingContext(ItemMappingContext);
				}
				//IPlayerInputInterface::Execute_PII_AddInputMappingContext(OtherActor, ItemMappingContext);
			}
		}

		// Check if OtherActor implements IReferencesInterface
		if (OtherActor->GetClass()->ImplementsInterface(UReferencesInterface::StaticClass()))
		{
			APlayerCharacter* PlayerCharacter = IReferencesInterface::Execute_RI_GetPlayerCharacter(OtherActor);
			if (PlayerCharacter)
			{
				PlayerCharacter->SetOverlappingItem(this);
			}
		}
	}
}

void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		// Check if OtherActor implements IPlayerInputInterface
		if (OtherActor->GetClass()->ImplementsInterface(UPlayerInputInterface::StaticClass()))
		{
			if (ItemMappingContext)
			{
				IPlayerInputInterface* PlayerInputInterface = Cast<IPlayerInputInterface>(OtherActor);
				if (PlayerInputInterface)
				{
					PlayerInputInterface->PII_RemoveInputMappingContext(ItemMappingContext);
				}
				//IPlayerInputInterface::Execute_PII_RemoveInputMappingContext(OtherActor, ItemMappingContext);
			}
		}

		// Check if OtherActor implements IReferencesInterface
		if (OtherActor->GetClass()->ImplementsInterface(UReferencesInterface::StaticClass()))
		{
			APlayerCharacter* PlayerCharacter = IReferencesInterface::Execute_RI_GetPlayerCharacter(OtherActor);
			if (PlayerCharacter)
			{
				PlayerCharacter->SetOverlappingItem(nullptr);
			}
		}
	}
}

float AItem::TransformedSin()
{
	return Amplitude * FMath::Sin(RunningTime * TimeConstant);
}

float AItem::TransformedCos()
{
	return Amplitude * FMath::Cos(RunningTime * TimeConstant);
}

void AItem::SimulatePhysics(bool bShouldSimulatePhysics)
{
	ItemMesh->SetSimulatePhysics(bShouldSimulatePhysics);
}

