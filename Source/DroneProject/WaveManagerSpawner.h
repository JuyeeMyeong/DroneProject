// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WaveManagerSpawner.generated.h"

class UWaveManagerComponent;

UCLASS()
class DRONEPROJECT_API AWaveManagerSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AWaveManagerSpawner();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wave")
	UWaveManagerComponent* WaveManager;
};
