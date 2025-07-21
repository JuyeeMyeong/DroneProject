// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnVolume.generated.h"

class UBoxComponent;
class ABaseItem;

UCLASS()
class DRONEPROJECT_API ASpawnVolume : public AActor
{
	GENERATED_BODY()

public:

    ASpawnVolume();

    UFUNCTION()
    ABaseItem* SpawnItem(TSubclassOf<ABaseItem> ItemClass);

protected:
    UPROPERTY(VisibleAnywhere, Category = "Spawn")
    class UBoxComponent* SpawnArea;
};
