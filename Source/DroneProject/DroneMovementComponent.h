// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "DroneMovementComponent.generated.h"

UCLASS(ClassGroup = Movement, meta = (BlueprintSpawnableComponent))
class DRONEPROJECT_API UDroneMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()

public:
	UDroneMovementComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void AddInputVector(const FVector& Input);

	FVector GetVelocity() const { return Velocity; }

protected:
	FVector Velocity;
	FVector Acceleration;
	FVector MovementInput;
	FVector AdjustedInput;
	FVector Force;

	FRotator AngularVelocity;
	FRotator AngularAcceleration;
	FRotator TorqueInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DronePhysics")
	float Mass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DronePhysics")
	float Gravity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DronePhysics")
	float MaxThrustForce;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DronePhysics")
	float MaxVerticalThrust;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DronePhysics")
	float DragCoefficient;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DronePhysics")
	float DampingSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DronePhysics")
	float MaxSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DronePhysics")
	float AirControlFactor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ground Check")
	float GroundCheckDistance;

	bool bIsOnGround;

	FVector PerformGroundCheck();

	void UpdateMovementInput();

	void CalculateDroneForce();

	void UpdateVelocity(float DeltaTime);

	void ClampVelocity();

	void HandleCollision(FHitResult& Hit, const FVector& DeltaMove);

	void MoveDrone(float DeltaTime);

	void ApplyRotation(float DeltaTime);

	void UpdateGroundState();

	void ClearInput();
};
