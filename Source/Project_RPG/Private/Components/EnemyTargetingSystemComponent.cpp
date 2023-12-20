// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/EnemyTargetingSystemComponent.h"
#include "Components/SphereComponent.h"
#include "Enemy/Enemy.h"

// Sets default values for this component's properties
UEnemyTargetingSystemComponent::UEnemyTargetingSystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// Create the sphere component
	DetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionSphere"));
	DetectionSphere->InitSphereRadius(500.0f); // Set the radius as needed
	DetectionSphere->SetCollisionProfileName(TEXT("TargetDetection"));
}


// Called when the game starts
void UEnemyTargetingSystemComponent::BeginPlay()
{
	Super::BeginPlay();

	DetectionSphere->SetupAttachment(GetOwner()->GetRootComponent());
	
	// Configure detection
	DetectionSphere->OnComponentBeginOverlap.AddDynamic(this, &UEnemyTargetingSystemComponent::OnDetectionSphereOverlap);
	DetectionSphere->OnComponentEndOverlap.AddDynamic(this, &UEnemyTargetingSystemComponent::OnDetectionSphereEndOverlap);
}

// Called every frame
void UEnemyTargetingSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UEnemyTargetingSystemComponent::OnDetectionSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
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