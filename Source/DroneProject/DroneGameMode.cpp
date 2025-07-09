// Fill out your copyright notice in the Description page of Project Settings.


#include "DroneGameMode.h"
#include "DronePlayer.h"
#include "DroneController.h"

ADroneGameMode::ADroneGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

	DefaultPawnClass = ADronePlayer::StaticClass();
	PlayerControllerClass = ADroneController::StaticClass();
}

