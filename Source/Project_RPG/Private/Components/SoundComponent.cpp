// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SoundComponent.h"
#include "Sound/SoundBase.h"
/* Game Framework */
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
/* Kismet */
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
USoundComponent::USoundComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USoundComponent::BeginPlay()
{
	Super::BeginPlay();

	ActorCheckClass = AActor::StaticClass();
	CharacterCheckClass = ACharacter::StaticClass();

	if (GetOwner()->GetClass()->IsChildOf(CharacterCheckClass))
	{
		OwnerCharacter = Cast<ACharacter>(GetOwner());
	}
	else
	{
		OwnerActor = GetOwner();
	}
}

// Called every frame
void USoundComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USoundComponent::PlayHitSound(const FVector& ImpactPoint)
{
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetOwner(), HitSound, ImpactPoint);
	}
}


