// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "DroneDataTables.generated.h"

//--- For Item Spawn ---//
USTRUCT(BlueprintType)
struct FSpawnCount
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave|Spawn")
    TSubclassOf<AActor> ItemClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave|Spawn")
    int32 Count;
};

//--- Item Data ---//
USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FName ItemID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    bool bStackable;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    float Duration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    UTexture2D* Icon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    UStaticMesh* Mesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    float SizeScalar;
};

//--- Configs for Wave ---//
USTRUCT(BlueprintType)
struct FWaveConfig : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
    int32 WaveIndex;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
    float TimeLimit;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
    TArray<FSpawnCount> SpawnCounts;
};
