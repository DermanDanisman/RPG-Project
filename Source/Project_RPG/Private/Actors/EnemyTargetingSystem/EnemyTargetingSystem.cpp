// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/EnemyTargetingSystem/EnemyTargetingSystem.h"
#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "Components/ArrowComponent.h"
#include "Enemy/Enemy.h"
/* Kismet */
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"


// Constructor
AEnemyTargetingSystem::AEnemyTargetingSystem()
{
    PrimaryActorTick.bCanEverTick = true;

    // Create the detection sphere component
    DetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionSphere"));
    DetectionSphere->SetupAttachment(GetRootComponent());
    DetectionSphere->InitSphereRadius(500.f); // Set the radius
    DetectionSphere->SetCollisionProfileName(TEXT("TargetDetection")); // Set collision profile
    DetectionSphere->SetHiddenInGame(false); // Sphere is visible in game
}

// Called at the start of the game or when spawned
void AEnemyTargetingSystem::BeginPlay()
{
    Super::BeginPlay();

    // Get the owner character of the system
    OwnerCharacter = Cast<ACharacter>(GetParentActor());
    EnemyFilterClass = AEnemy::StaticClass();

    // Set up overlap events for the detection sphere
    DetectionSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemyTargetingSystem::OnDetectionSphereOverlap);
    DetectionSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemyTargetingSystem::OnDetectionSphereEndOverlap);
}

// Called every frame
void AEnemyTargetingSystem::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (CurrentTarget)
    {
        // Get the current control rotation
        FRotator ControlRotation = OwnerCharacter->GetControlRotation();
        FVector OwnerCharacterLocation = OwnerCharacter->GetActorLocation();

        // Calculate the LookAt rotation towards the target
        FRotator LookAt = UKismetMathLibrary::FindLookAtRotation(OwnerCharacterLocation, CurrentTarget->GetActorLocation());

        // Interpolate only the Yaw component to smoothly rotate towards the target
        FRotator LookAtInterp = UKismetMathLibrary::RInterpTo(ControlRotation, FRotator(0.f, LookAt.Yaw, 0.f), DeltaTime, 8.0f);

        // Combine the original Pitch with the new Yaw
        FRotator NewControlRotation = FRotator(ControlRotation.Pitch, LookAtInterp.Yaw, 0.f);

        // Set the new control rotation
        OwnerCharacter->GetController()->SetControlRotation(NewControlRotation);
    }
}

// Handle new targets entering the detection sphere
void AEnemyTargetingSystem::OnDetectionSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // Add valid targets to the target list
    if (IsValidTarget(OtherActor))
    {
        TargetList.AddUnique(OtherActor);
    }

    if (CurrentTarget)
    {
       // GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("Current Target Name: %s"), *CurrentTarget->GetName()));
    }
}

// Handle targets leaving the detection sphere
void AEnemyTargetingSystem::OnDetectionSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    // Remove the actor from the target list when it exits the sphere
    if (IsValid(OtherActor))
    {
        if (!TargetList.IsEmpty())
        {
            int Index = TargetList.Find(OtherActor);
            TargetList.RemoveAt(Index);

            /*if (CurrentTarget)
            {
                GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("Current Target Name: %s"), *CurrentTarget->GetName()));
                GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("Other Actor Target: %s"), *OtherActor->GetName()));
            }*/

            // Only clear the current target if the actor leaving the sphere is the current target
            if (CurrentTarget == OtherActor)
            {
                ClearCurrentTarget();
                //GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("Cleared Current Target")));
            }
        }
    }
}

// Update the target list and find the target in the line of sight
void AEnemyTargetingSystem::GetTargetsInRange()
{
    // Update target list based on actors within the detection sphere
    TargetList.Empty();
    DetectionSphere->GetOverlappingActors(TargetList, EnemyFilterClass);
}

// Find a target in the line of sight from a list of potential targets
AActor* AEnemyTargetingSystem::GetTargetInLineOfSight(const TArray<AActor*>& PotentialTargets)
{
    if (!PotentialTargets.IsEmpty())
    {
        if (!OwnerCharacter) return nullptr;

        FVector PlayerViewLocation;
        FRotator PlayerViewRotation;
        OwnerCharacter->GetController()->GetPlayerViewPoint(PlayerViewLocation, PlayerViewRotation);

        // Use the character's location as the starting point of the trace
        FVector CharacterLocation = OwnerCharacter->GetActorLocation() + FVector(0.f, 0.f, 50.f);

        FVector ViewDirection = PlayerViewRotation.Vector();
        FVector TraceEnd = CharacterLocation + (ViewDirection * 500.f); // Adjust the range as needed

        TArray<AActor*> IgnoreActors;
        IgnoreActors.Add(this);
        IgnoreActors.Add(OwnerCharacter);

        float Radius = 15.f;
        float HalfHeight = 50.f;

        FHitResult HitResult;
        bool bHit = UKismetSystemLibrary::CapsuleTraceSingle(
            this,
            CharacterLocation, // Start location should be PlayerViewLocation
            TraceEnd,
            Radius,
            HalfHeight,
            ETraceTypeQuery::TraceTypeQuery1,
            false,
            IgnoreActors,
            EDrawDebugTrace::ForDuration,
            HitResult,
            true,
            FLinearColor::Red,
            FLinearColor::Green,
            5.0f
        );

        if (bHit && IsValidTarget(HitResult.GetActor()))
        {
            return HitResult.GetActor(); // Return the actor if it's a valid target
        }
        /*else if (!bHit)
        {
            if (!bHit || !IsValidTarget(HitResult.GetActor()))
            {
                return GetNearestTarget(PotentialTargets);
            }
        }*/
    }

    return nullptr;
}

AActor* AEnemyTargetingSystem::GetNearestTarget(const TArray<AActor*>& PotentialTargets)
{
    if (!OwnerCharacter) return nullptr;

    AActor* NearestTarget = nullptr;
    float NearestDistance = FLT_MAX;

    for (AActor* TargetActor : PotentialTargets)
    {
        if (IsValidTarget(TargetActor))
        {
            float Distance = TargetActor->GetDistanceTo(OwnerCharacter);
            if (Distance < NearestDistance)
            {
                NearestDistance = Distance;
                NearestTarget = TargetActor;
                //GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("NearestTarget: %s"), *NearestTarget->GetName()));
            }
        }
    }

    return NearestTarget;
}

// Determine if an actor is a valid target
bool AEnemyTargetingSystem::IsValidTarget(AActor* TargetActor)
{
    if (IsValid(TargetActor) && TargetActor->GetClass()->IsChildOf(EnemyFilterClass) && TargetList.Contains(TargetActor))
    {
        return true; // Actor is a valid target
    }
    return false; // Actor is not a valid target
}

AActor* AEnemyTargetingSystem::GetNextTarget()
{
    if (TargetList.IsEmpty()) return nullptr;

    int32 CurrentTargetIndex = TargetList.IndexOfByKey(CurrentTarget);
    int32 NextTargetIndex = (CurrentTargetIndex + 1) % TargetList.Num();

    return TargetList[NextTargetIndex];
}

void AEnemyTargetingSystem::SelectTarget(AActor* NewTarget)
{
    GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("Select Target Function")));
    if (!TargetList.IsEmpty())
    {
        if (NewTarget)
        {
            // Clear existing target indicator if it exists
            if (CurrentTargetIndicator)
            {
                CurrentTargetIndicator->DestroyComponent();
                CurrentTargetIndicator = nullptr;
            }

            // Set the new target
            CurrentTarget = NewTarget;

            // Spawn the arrow component above the new target
            if (CurrentTarget == GetTargetInLineOfSight(TargetList))
            {
                SpawnTargetIndicator(CurrentTarget);
            }
        }
    }
}

void AEnemyTargetingSystem::SpawnTargetIndicator(AActor* Target)
{
    FVector TargetLocation = Target->GetActorLocation();
    FVector IndicatorLocation = TargetLocation + FVector(0, 0, 130.f); // Adjust this offset as needed

    CurrentTargetIndicator = NewObject<UArrowComponent>(this);
    CurrentTargetIndicator->RegisterComponent();
    CurrentTargetIndicator->AttachToComponent(Target->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
    CurrentTargetIndicator->SetWorldLocation(IndicatorLocation);
    CurrentTargetIndicator->SetWorldRotation(FRotator(-90.f, 0, 0));
    CurrentTargetIndicator->SetHiddenInGame(false);
    // Additional setup for the indicator as needed
}

void AEnemyTargetingSystem::ClearCurrentTarget()
{
    CurrentTarget = nullptr;

    if (CurrentTargetIndicator)
    {
        CurrentTargetIndicator->DestroyComponent();
        CurrentTargetIndicator = nullptr;
    }
}