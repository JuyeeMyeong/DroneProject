// Fill out your copyright notice in the Description page of Project Settings.


#include "DroneMovementComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"

UDroneMovementComponent::UDroneMovementComponent()
	: Gravity(-980.0f)
{
	PrimaryComponentTick.bCanEverTick = true;

	Velocity = FVector::ZeroVector;
	Acceleration = FVector::ZeroVector;
	MovementInput = FVector::ZeroVector;
	AdjustedInput = FVector::ZeroVector;
	Force = FVector::ZeroVector;

	AngularVelocity = FRotator::ZeroRotator;
	AngularAcceleration = FRotator::ZeroRotator;
	TorqueInput = FRotator::ZeroRotator;

	Mass = 1.0f;
	MaxThrustForce = 1500.0f;
	MaxVerticalThrust = 2000.0f;
	DragCoefficient = 0.2f;
	DampingSpeed = 2.0f;
	MaxSpeed = 2000.0f;
	AirControlFactor = 0.3f;
	GroundCheckDistance = 100.0f;

	bIsOnGround = false;
}

void UDroneMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	Gravity = GetWorld()->GetGravityZ();
}

void UDroneMovementComponent::AddInputVector(const FVector& Input)
{
	MovementInput = Input;
}

FVector UDroneMovementComponent::PerformGroundCheck()
{
	FVector Start = UpdatedComponent->GetComponentLocation();
	FVector End = Start - FVector(0.f, 0.f, GroundCheckDistance);

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());

	bool bHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		Start,
		End,
		FQuat::Identity,
		ECC_Visibility,
		FCollisionShape::MakeSphere(10.0f),
		Params
	);

	bIsOnGround = bHit && HitResult.Normal.Z > 0.55f;
	return HitResult.ImpactPoint;
}

void UDroneMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!PawnOwner || !UpdatedComponent) return;

	UpdateMovementInput();
	CalculateDroneForce();
	UpdateVelocity(DeltaTime);
	ClampVelocity();

	MoveDrone(DeltaTime);
	ApplyRotation(DeltaTime);

	UpdateGroundState();
	ClearInput();
}

void UDroneMovementComponent::UpdateMovementInput()
{
	float ControlFactor = bIsOnGround ? 1.0f : AirControlFactor;
	AdjustedInput.X = MovementInput.X * ControlFactor;
	AdjustedInput.Y = MovementInput.Y * ControlFactor;

	float ZInput = MovementInput.Z;
	AdjustedInput.Z = (!(bIsOnGround && ZInput <= 0.f)) ? ZInput : 0.f;
}

void UDroneMovementComponent::CalculateDroneForce()
{
	const FVector Forward = PawnOwner->GetActorForwardVector();
	const FVector Right = PawnOwner->GetActorRightVector();
	const FVector Up = PawnOwner->GetActorUpVector();

	Force = FVector::ZeroVector;
	Force += Forward * AdjustedInput.X * MaxThrustForce;
	Force += Right * AdjustedInput.Y * MaxThrustForce;
	Force += Up * AdjustedInput.Z * MaxVerticalThrust;

	FVector WorldGravity = FVector(0.f, 0.f, Gravity);
	FVector LocalGravity = GetOwner()->GetActorRotation().UnrotateVector(WorldGravity);
	Force += LocalGravity * Mass;
	Force += -Velocity * DragCoefficient;
}

void UDroneMovementComponent::UpdateVelocity(float DeltaTime)
{
	Acceleration = Force / Mass;
	Velocity += Acceleration * DeltaTime;

	if (MovementInput.IsNearlyZero())
	{
		Velocity = FMath::VInterpTo(Velocity, FVector::ZeroVector, DeltaTime, DampingSpeed);
	}
}

void UDroneMovementComponent::ClampVelocity()
{
	if (Velocity.Size() > MaxSpeed)
	{
		Velocity = Velocity.GetSafeNormal() * MaxSpeed;
	}
}

void UDroneMovementComponent::HandleCollision(FHitResult& Hit, const FVector& DeltaMove)
{
	const FVector Normal = Hit.Normal;
	const float UpDot = FVector::DotProduct(Normal, FVector::UpVector);

	const FVector ImpactPoint = Hit.ImpactPoint;

	if (UpDot > 0.7f)
	{
		if (Velocity.Z < 0.f)
			Velocity.Z = 0.f;
	}
	else if (UpDot < -0.7f)
	{
		if (Velocity.Z > 0.f)
			Velocity.Z = 0.f;
	}
	else
	{
		FVector Slide = ComputeSlideVector(DeltaMove, 1.f - Hit.Time, Normal, Hit);
		Velocity = Slide / GetWorld()->GetDeltaSeconds();
	}

	SlideAlongSurface(DeltaMove, 1.f - Hit.Time, Normal, Hit);
}

void UDroneMovementComponent::MoveDrone(float DeltaTime)
{
	FVector DeltaMove = Velocity * DeltaTime;
	FHitResult Hit;
	SafeMoveUpdatedComponent(DeltaMove, UpdatedComponent->GetComponentRotation(), true, Hit);
	if (Hit.IsValidBlockingHit())
	{
		HandleCollision(Hit, DeltaMove);
	}
}

void UDroneMovementComponent::ApplyRotation(float DeltaTime)
{
	AngularVelocity += TorqueInput * DeltaTime;
	FRotator RotationDelta = AngularVelocity * DeltaTime;
	UpdatedComponent->AddLocalRotation(FQuat(RotationDelta));
}

void UDroneMovementComponent::UpdateGroundState()
{
	PerformGroundCheck();
}

void UDroneMovementComponent::ClearInput()
{
	MovementInput = FVector::ZeroVector;
	TorqueInput = FRotator::ZeroRotator;
}
