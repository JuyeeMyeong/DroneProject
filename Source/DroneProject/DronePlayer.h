// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/ArrowComponent.h"
#include "DronePlayer.generated.h"

class USphereComponent;
class USkelMeshComponent;
class UInputComponent;
class UCameraComponent;
class USpringArmComponent;
class UDroneMovementComponent;
class UHealthComponent;
class UScoreComponent;
struct FInputActionValue;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnItemUsed, UTexture2D*, Icon, float, Duration, FText, Description);

UCLASS()
class DRONEPROJECT_API ADronePlayer : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ADronePlayer();

	UPROPERTY(BlueprintAssignable, Category = "Item")
	FOnItemUsed OnItemUsed;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "General", meta = (AllowPrivateAccess = "true"))
	UArrowComponent* ArrowComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	USphereComponent* CollisionComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* MeshComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CameraComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom Movement", meta = (AllowPrivateAccess = "true"))
	UDroneMovementComponent* DroneMovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category ="Health", meta = (AllowPrivateAccess = "true"))
	UHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Score", meta = (AllowPrivateAccess = "true"))
	UScoreComponent* ScoreComponent;

	float RollSpeed;

	bool bIsInverted = false;

	FTimerHandle PlayerTimer;

protected:
	UFUNCTION()
	void Move(const FInputActionValue& Value);
	UFUNCTION()
	void Look(const FInputActionValue& Value);
	UFUNCTION()
	void Roll(const FInputActionValue& Value);

public:
	void InvertControls(float Duration);
};
