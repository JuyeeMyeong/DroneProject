// Fill out your copyright notice in the Description page of Project Settings.


#include "DronePlayer.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "DroneMovementComponent.h"
#include "DroneController.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/PlayerController.h"

// Sets default values
ADronePlayer::ADronePlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->InitSphereRadius(60.0f);
	CollisionComponent->SetCollisionProfileName(TEXT("BlockAll"));
	SetRootComponent(CollisionComponent);
	CollisionComponent->SetSimulatePhysics(false);

	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetSimulatePhysics(false);

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(MeshComponent);
	SpringArmComponent->TargetArmLength = 300.0f;
	SpringArmComponent->bUsePawnControlRotation = false;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;

	DroneMovementComponent = CreateDefaultSubobject<UDroneMovementComponent>(TEXT("DroneMovementComponent"));
	DroneMovementComponent->UpdatedComponent = CollisionComponent;

	RollSpeed = 60.0f;
}

// Called when the game starts or when spawned
void ADronePlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADronePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (DroneMovementComponent)
	{
		SpringArmComponent->SetWorldRotation(GetActorRotation());
	}
}

// Called to bind functionality to input
void ADronePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (ADroneController* DroneController = Cast<ADroneController>(GetController()))
		{
			if (DroneController->MoveAction)
			{
				EnhancedInputComponent->BindAction(DroneController->MoveAction, ETriggerEvent::Triggered, this, &ADronePlayer::Move);
			}
			if (DroneController->LookAction)
			{
				EnhancedInputComponent->BindAction(DroneController->LookAction, ETriggerEvent::Triggered, this, &ADronePlayer::Look);
			}
			if (DroneController->RollAction)
			{
				EnhancedInputComponent->BindAction(DroneController->RollAction, ETriggerEvent::Triggered, this, &ADronePlayer::Roll);
			}
		}
	}
}

void ADronePlayer::Move(const FInputActionValue& Value)
{
	if (!Controller) return;

	FVector Input = Value.Get<FVector>();
	DroneMovementComponent->AddInputVector(Input);
}

void ADronePlayer::Look(const FInputActionValue& Value)
{
	FVector2D Input = Value.Get<FVector2D>();
	if (Input.IsNearlyZero()) return;

	FRotator LookRotation(Input.Y, Input.X, 0.f); // Pitch, Yaw
	AddActorLocalRotation(FQuat(LookRotation));

	SpringArmComponent->SetWorldRotation(GetActorRotation());
}

void ADronePlayer::Roll(const FInputActionValue& Value)
{
	float Input = Value.Get<float>();
	if (FMath::IsNearlyZero(Input)) return;

	FRotator RollDelta(0.f, 0.f, Input * RollSpeed * GetWorld()->GetDeltaSeconds());
	AddActorLocalRotation(FQuat(RollDelta));
}