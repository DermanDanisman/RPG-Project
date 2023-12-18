// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ParticleEffectComponent.h"
/* Game Framework */
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
/* Particles */
#include "Particles/ParticleSystemComponent.h"
/* Kismet */
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UParticleEffectComponent::UParticleEffectComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UParticleEffectComponent::BeginPlay()
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
void UParticleEffectComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UParticleEffectComponent::SpawnHitParticleEffect(const FVector& ImpactPoint)
{
	if (HitParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetOwner(), HitParticles, ImpactPoint);
	}
}


