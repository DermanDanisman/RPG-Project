// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterMontageComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_RPG_API UCharacterMontageComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterMontageComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:

	/* References */
	UPROPERTY(VisibleAnywhere, Category = "References")
	class ACharacter* OwnerCharacter = nullptr;

public:

	UFUNCTION()
	void PlayMontageFromSection(UAnimMontage* Montage, const FName& SectionName = FName(TEXT("Default")));

	UFUNCTION()
	void PlayHitReactionMontage(const FVector& ImpactPoint);

	/*UFUNCTION()
	void PlayDodgeMontage();*/
	
private:

	UPROPERTY(EditAnywhere, Category = "Montages")
	class UAnimMontage* HitReactionMontage = nullptr;

	/*UPROPERTY(EditAnywhere, Category = "Montages")
	class UAnimMontage* DodgeMontage = nullptr;*/
};
