// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/EnemyTargetingSystemComponent.h"
#include "GameFramework/Character.h"
#include "Enemy/Enemy.h"
#include "Components/SphereComponent.h"



// Sets default values for this component's properties
UEnemyTargetingSystemComponent::UEnemyTargetingSystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UEnemyTargetingSystemComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
}

// Called every frame
void UEnemyTargetingSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UEnemyTargetingSystemComponent::SetDetectionSphere(USphereComponent* Sphere)
{
	if (Sphere)
	{
		DetectionSphere = Sphere;
		// Configure detection
		DetectionSphere->OnComponentBeginOverlap.AddDynamic(this, &UEnemyTargetingSystemComponent::OnDetectionSphereOverlap);
		DetectionSphere->OnComponentEndOverlap.AddDynamic(this, &UEnemyTargetingSystemComponent::OnDetectionSphereEndOverlap);

	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("Sphere is NOT VALID!")));
	}
}

void UEnemyTargetingSystemComponent::OnDetectionSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("Other Actor: %s"), *OtherActor->GetName()));
	AEnemy* Enemy = Cast<AEnemy>(OtherActor);
	if (Enemy)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("Enemy: %s"), *Enemy->GetName()));
		// Enemy is a valid target
		// You might want to store this enemy in a list of detected enemies
	}
}

void UEnemyTargetingSystemComponent::OnDetectionSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}
