// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/Bird.h"
/* Components */
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
/* Game Framework */
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"
/* Camera */
#include "Camera/CameraComponent.h"

// Sets default values
ABird::ABird()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("UCapsuleComponent"));
	CapsuleComponent->SetCapsuleHalfHeight(15.f);
	CapsuleComponent->SetCapsuleRadius(15.f);
	CapsuleComponent->SetCollisionProfileName(FName("BlockAll"));
	SetRootComponent(CapsuleComponent);

	BirdMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("USkeletalMeshComponent"));
	BirdMesh->SetupAttachment(GetRootComponent());

	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("USpringArmComponent"));
	CameraSpringArm->SetupAttachment(GetRootComponent());
	CameraSpringArm->TargetArmLength = 150.f;


	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("UCameraComponent"));
	ViewCamera->SetupAttachment(CameraSpringArm);

	FloatingPawnMovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("UFloatingPawnMovement"));
	FloatingPawnMovementComponent->UpdatedComponent = GetRootComponent();

	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw = true;

}

// Called when the game starts or when spawned
void ABird::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABird::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABird::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

