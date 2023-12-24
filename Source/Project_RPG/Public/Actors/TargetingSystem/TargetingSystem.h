// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TargetingSystem.generated.h"

UCLASS()
class PROJECT_RPG_API ATargetingSystem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATargetingSystem();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/* Delegate Binded Functions */
	UFUNCTION()
	virtual void OnDetectionSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnDetectionSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:

	UFUNCTION()
	FORCEINLINE AActor* GetCurrentTarget() { return CurrentTarget; }

	UFUNCTION()
	FORCEINLINE TArray<AActor*> GetTargetList() { return TargetList; }

	UPROPERTY()
	class ACharacter* OwnerCharacter = nullptr;

	UFUNCTION()
	void GetTargetsInRange();

	UFUNCTION()
	AActor* GetTargetInLineOfSight(const TArray<AActor*>& PotentialTargets);

	UFUNCTION()
	void GetNextTarget(float SearchValue);

	UFUNCTION()
	void SelectTarget(AActor* NewTarget);

	UFUNCTION()
	void ClearCurrentTarget();

private:

	UFUNCTION()
	AActor* GetNearestTarget(const TArray<AActor*>& PotentialTargets);

	UFUNCTION()
	bool IsValidTarget(AActor* TargetActor);

	UFUNCTION()
	void SpawnTargetIndicator(AActor* Target);

private:	

	UPROPERTY(VisibleAnywhere, Category = "Targeting")
	class USphereComponent* DetectionSphere;

	UPROPERTY(EditAnywhere, Category = "Filtering Class")
	TSubclassOf<class AEnemy> EnemyFilterClass;

	UPROPERTY(VisibleAnywhere, Category = "Targets")
	TArray<AActor*> TargetList;

	UPROPERTY(VisibleAnywhere, Category = "Targets")
	AActor* CurrentTarget = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Visual Indicator")
	class UArrowComponent* CurrentTargetIndicator = nullptr;

	// Add this Mapping Context when in contact with items
	UPROPERTY(EditAnywhere, Category = "Input Mapping Context")
	class UInputMappingContext* CombatMappingContext = nullptr;
};
