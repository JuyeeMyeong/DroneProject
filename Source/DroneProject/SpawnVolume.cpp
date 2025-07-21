// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnVolume.h"
#include "Components/BoxComponent.h"
#include "BaseItem.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"


ASpawnVolume::ASpawnVolume()
{
    PrimaryActorTick.bCanEverTick = false;
    SpawnArea = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnArea"));
    RootComponent = SpawnArea;
}

ABaseItem* ASpawnVolume::SpawnItem(TSubclassOf<ABaseItem> ItemClass)
{
    if (!ItemClass) return nullptr;

    FVector Origin = SpawnArea->GetComponentLocation();
    FVector Extent = SpawnArea->GetScaledBoxExtent();
    FVector RandomPoint = UKismetMathLibrary::RandomPointInBoundingBox(Origin, Extent);

    FActorSpawnParameters Params;
    ABaseItem* Spawned = GetWorld()->SpawnActor<ABaseItem>(ItemClass, RandomPoint, FRotator::ZeroRotator, Params);
    return Spawned;
}