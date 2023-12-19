// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ParticleEffectComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_RPG_API UParticleEffectComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UParticleEffectComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	UFUNCTION()
	void SpawnHitParticleEffect(const FVector& ImpactPoint);

private:

	UPROPERTY(VisibleAnywhere, Category = "References")
	class AActor* OwnerActor = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "References")
	TSubclassOf<AActor> ActorCheckClass = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "References")
	class ACharacter* OwnerCharacter = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "References")
	TSubclassOf<ACharacter> CharacterCheckClass = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Hit Effects")
	class UParticleSystem* HitParticles = nullptr;

		
};