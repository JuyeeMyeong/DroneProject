// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "DroneGameMode.generated.h"

class UWaveManagerComponent;

UCLASS()
class DRONEPROJECT_API ADroneGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ADroneGameMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
