// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ParticleEffectComponent.h"
/* Game Framework */
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
/* Particles */
#include "Particles/ParticleSystemComponent.h"
/* Kismet */
#include "Kismet/GameplayStatics.h"
/* Components */
#include "Components/StaticMeshComponent.h"

// Sets default values for this component's properties
UParticleEffectComponent::UParticleEffectComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
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
		OwnerActorStaticMesh = Cast<UStaticMeshComponent>(OwnerActor->GetComponentByClass(UStaticMeshComponent::StaticClass()));
	}
}

// Called every frame
void UParticleEffectComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, FString::Printf(TEXT("Particle Effect Component Tick")));
}

// When Character is hit spawn Hit Particle Effect
void UParticleEffectComponent::SpawnHitParticleEffect(const FVector& ImpactPoint)
{
	if (HitParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(OwnerCharacter, HitParticles, ImpactPoint);
	}
}


