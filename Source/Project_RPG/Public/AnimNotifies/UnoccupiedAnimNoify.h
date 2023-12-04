// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Interfaces/ReferencesInterface.h"
#include "UnoccupiedAnimNoify.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_RPG_API UUnoccupiedAnimNoify : public UAnimNotify, public IReferencesInterface
{
	GENERATED_BODY()
	
public:

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
