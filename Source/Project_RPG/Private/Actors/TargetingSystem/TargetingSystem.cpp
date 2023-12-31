// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/TargetingSystem/TargetingSystem.h"
#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "Components/ArrowComponent.h"
#include "Enemy/Enemy.h"
/* Kismet */
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"


// Constructor
ATargetingSystem::ATargetingSystem()
{
    PrimaryActorTick.bCanEverTick = true;

    // Create the detection sphere component
    DetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionSphere"));
    DetectionSphere->SetupAttachment(GetRootComponent());
    DetectionSphere->InitSphereRadius(500.f); // Set the radius
    DetectionSphere->SetCollisionProfileName(TEXT("TargetDetection")); // Set collision profile
    DetectionSphere->SetGenerateOverlapEvents(true);
    DetectionSphere->SetHiddenInGame(false); // Sphere is visible in game
}

// Called at the start of the game or when spawned
void ATargetingSystem::BeginPlay()
{
    Super::BeginPlay();

    // Get the owner character of the system
    OwnerCharacter = Cast<ACharacter>(GetParentActor());
    EnemyFilterClass = AEnemy::StaticClass();

    // Set up overlap events for the detection sphere
    DetectionSphere->OnComponentBeginOverlap.AddDynamic(this, &ATargetingSystem::OnDetectionSphereOverlap);
    DetectionSphere->OnComponentEndOverlap.AddDynamic(this, &ATargetingSystem::OnDetectionSphereEndOverlap);
}

// Called every frame
void ATargetingSystem::Tick(float DeltaTime)
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
        //FRotator LookAtInterp = UKismetMathLibrary::RInterpTo(ControlRotation, FRotator(0.f, LookAt.Yaw, 0.f), DeltaTime, 3.0f);

        // Combine the original Pitch with the new Yaw
        FRotator NewControlRotation = FRotator(ControlRotation.Pitch, LookAt.Yaw, 0.f);

        // Set the new control rotation
        OwnerCharacter->GetController()->SetControlRotation(NewControlRotation);
    }
}

// Handle new targets entering the detection sphere. If the overlapped actor is a valid target, it's added to TargetList.
void ATargetingSystem::OnDetectionSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // Add valid targets to the target list
    if (OtherActor->GetClass()->IsChildOf(EnemyFilterClass))
    {
        TargetList.AddUnique(OtherActor);   
    }
}

// Handle targets leaving the detection sphere. If the actor leaving the sphere is the current target, the current target is cleared.
void ATargetingSystem::OnDetectionSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    // Remove the actor from the target list when it exits the sphere
    if (IsValid(OtherActor))
    {
        TargetList.RemoveSingle(OtherActor);

        // Only clear the current target if the actor leaving the sphere is the current target
        if (CurrentTarget == OtherActor)
        {
            ClearCurrentTarget();
        }
    }
}

// Empties and then updates TargetList based on actors currently overlapping the detection sphere.
void ATargetingSystem::GetTargetsInRange()
{
    // Update target list based on actors within the detection sphere
    TargetList.Empty();
    DetectionSphere->GetOverlappingActors(TargetList, EnemyFilterClass);
}

// Find a target in the line of sight from a list of potential targets
// Sorts potential targets based on their distance to the OwnerCharacter.
// Uses a capsule trace to find the first target in line of sight.
// Returns the first valid target hit by the trace.
AActor* ATargetingSystem::GetTargetInLineOfSight(const TArray<AActor*>& PotentialTargets)
{
    if (!OwnerCharacter || PotentialTargets.IsEmpty()) return nullptr;

    TArray<AActor*> IgnoreActors = { this, OwnerCharacter };
    float Radius = 15.f;
    float HalfHeight = 50.f;

    FVector PlayerViewLocation;
    FRotator PlayerViewRotation;
    OwnerCharacter->GetController()->GetPlayerViewPoint(PlayerViewLocation, PlayerViewRotation);

    FVector ViewDirection = PlayerViewRotation.Vector();
    FVector CharacterLocation = OwnerCharacter->GetActorLocation() + FVector(0.f, 0.f, 50.f);

    /*
    Sort is a template function provided by Unreal Engine's TArray class.
    It sorts the elements of the array based on a provided comparison function or lambda expression.
    In this code, Sort is used to order PotentialTargets based on their distance from the OwnerCharacter.
    The lambda expression [](const AActor& A, const AActor& B) is the comparison function. It returns true if Actor A is closer to the character than Actor B.
    Sort rearranges SortedTargets so that the closest target is first.
    */
    TArray<AActor*> SortedTargets = PotentialTargets;
    SortedTargets.Sort([CharacterLocation](const AActor& A, const AActor& B) {
        return (A.GetActorLocation() - CharacterLocation).SizeSquared() <
            (B.GetActorLocation() - CharacterLocation).SizeSquared();
        });

    // Iterate through the sorted targets
    for (AActor* TargetActor : SortedTargets)
    {
        if (!IsValidTarget(TargetActor)) continue;

        FVector TraceEnd = CharacterLocation + (ViewDirection * (TargetActor->GetActorLocation() - CharacterLocation).Size());
        FHitResult HitResult;
        bool bHit = UKismetSystemLibrary::CapsuleTraceSingle(
            this,
            CharacterLocation,
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

        if (bHit && HitResult.GetActor() == TargetActor)
        {
            return TargetActor; // Return the first valid target hit by the trace
        }
    }

    return nullptr; // No valid targets in line of sight
}

// Determines the closest target to the OwnerCharacter from the list of potential targets.
// Iterates through PotentialTargets, calculating the distance to each, and keeps track of the nearest one.
AActor* ATargetingSystem::GetNearestTarget(const TArray<AActor*>& PotentialTargets)
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
            }
        }
    }

    return NearestTarget;
}

// Checks if a given actor is a valid target based on certain criteria (e.g., class type and presence in the target list).
bool ATargetingSystem::IsValidTarget(AActor* TargetActor)
{
    if (IsValid(TargetActor) && TargetActor->GetClass()->IsChildOf(EnemyFilterClass) && TargetList.Contains(TargetActor))
    {
        return true; // Actor is a valid target
    }
    return false; // Actor is not a valid target
}

// Cycles through TargetList to find the next target in line.
// Uses the index of the current target to determine the next target.
void ATargetingSystem::GetNextTarget(float SearchValue)
{
    if (!TargetList.IsEmpty())
    {
        if (CurrentTarget)
        {
            int32 CurrentTargetIndex = TargetList.IndexOfByKey(CurrentTarget);
            int32 NextTargetIndex;

            if (SearchValue > 0)
            {
                // Move to the next target in the list
                NextTargetIndex = (CurrentTargetIndex + 1) % TargetList.Num();
            }
            else
            {
                // Move to the previous target in the list
                NextTargetIndex = (CurrentTargetIndex - 1 + TargetList.Num()) % TargetList.Num();
            }

            // Log for debugging
            GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, FString::Printf(TEXT("Get Next Target Index: %d"), NextTargetIndex));

            // Clear the current target and select the new one
            SelectTarget(TargetList[NextTargetIndex]);
            SpawnTargetIndicator(TargetList[NextTargetIndex]);
        }
    }
}

//void ATargetingSystem::UpdateTargetBasedOnViewDirection()
//{
//    if (!OwnerCharacter) return;
//
//    FVector PlayerViewLocation;
//    FRotator PlayerViewRotation;
//    OwnerCharacter->GetController()->GetPlayerViewPoint(PlayerViewLocation, PlayerViewRotation);
//
//    FVector ViewDirection = PlayerViewRotation.Vector();
//
//    AActor* BestTarget = nullptr;
//    float BestScore = FLT_MAX; // Lower scores are better
//
//    for (AActor* TargetActor : TargetList)
//    {
//        if (IsValidTarget(TargetActor))
//        {
//            FVector DirectionToTarget = (TargetActor->GetActorLocation() - PlayerViewLocation).GetSafeNormal();
//            float DotProduct = FVector::DotProduct(ViewDirection, DirectionToTarget);
//
//            // Adjust this threshold to control the field of view for target selection
//            const float ViewAngleThreshold = cos(FMath::DegreesToRadians(45.0f));
//
//            if (DotProduct > ViewAngleThreshold)
//            {
//                float Distance = (TargetActor->GetActorLocation() - PlayerViewLocation).Size();
//                if (Distance < BestScore)
//                {
//                    BestScore = Distance;
//                    BestTarget = TargetActor;
//                }
//            }
//        }
//    }
//
//    if (BestTarget)
//    {
//        // Switch to the new target
//        SelectTarget(BestTarget);
//        SpawnTargetIndicator(BestTarget);
//    }
//}

// Sets CurrentTarget to the specified target.
// Manages the spawning and attachment of an Target Indicator as an indicator over the current target.
void ATargetingSystem::SelectTarget(AActor* NewTarget)
{
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
            else
            {
                SpawnTargetIndicator(CurrentTarget);
            }
        }
    }
}

// Spawns and configures an arrow component above the specified target.
// Sets the location and orientation of the arrow to indicate the current target.
void ATargetingSystem::SpawnTargetIndicator(AActor* Target)
{
    if (Target)
    {   
        // Clear existing target indicator if it exists
        if (CurrentTargetIndicator)
        {
            CurrentTargetIndicator->DestroyComponent();
            CurrentTargetIndicator = nullptr;
        }

        FVector TargetLocation = Target->GetActorLocation();
        FVector IndicatorLocation = TargetLocation + FVector(0, 0, 160.f); // Adjust this offset as needed

        CurrentTargetIndicator = NewObject<UArrowComponent>(this);
        CurrentTargetIndicator->RegisterComponent();
        CurrentTargetIndicator->AttachToComponent(Target->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
        CurrentTargetIndicator->SetWorldLocation(IndicatorLocation);
        CurrentTargetIndicator->SetWorldRotation(FRotator(-90.f, 0, 0));
        CurrentTargetIndicator->SetHiddenInGame(false);
        // Additional setup for the indicator as needed
    }
}

// Clears the current target and removes the target indicator if it exists.
void ATargetingSystem::ClearCurrentTarget()
{
    CurrentTarget = nullptr;

    if (CurrentTargetIndicator)
    {
        CurrentTargetIndicator->DestroyComponent();
        CurrentTargetIndicator = nullptr;
        //bLockedOnTarget = false;
    }
}