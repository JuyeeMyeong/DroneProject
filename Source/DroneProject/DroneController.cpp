// Fill out your copyright notice in the Description page of Project Settings.


#include "DroneController.h"
#include "EnhancedInputSubsystems.h"


ADroneController::ADroneController(const FObjectInitializer& ObjectInitializer)
	: InputMappingContext(nullptr),
	MoveAction(nullptr),
	LookAction(nullptr),
	RollAction(nullptr)
{
}

void ADroneController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSubSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMappingContext)
			{
				InputSubSystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}
}
