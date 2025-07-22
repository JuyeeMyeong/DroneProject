// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DroneDataTables.h"
#include "WaveManagerComponent.generated.h"

class FActorComponent;
class UDataTable;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWaveEvent, int32, WaveIndex, float, TimeLimit);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), BlueprintType, Blueprintable)
class DRONEPROJECT_API UWaveManagerComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UWaveManagerComponent();

    UPROPERTY(EditAnywhere, Category = "Wave")
    UDataTable* WaveConfigTable;

    UPROPERTY(BlueprintAssignable, Category = "Wave")
    FOnWaveEvent OnWaveStarted;

    UPROPERTY(BlueprintReadOnly, Category = "Wave")
    int32 CurrentWaveIndex;
    UPROPERTY(BlueprintReadOnly, Category = "Wave")
    float WaveElapsedTime;

    UPROPERTY()
    TArray<class ASpawnVolume*> SpawnVolumes;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
    UDataTable* ItemDataTable;

protected:
    UFUNCTION()
    void RegisterSpawnVolume(ASpawnVolume* Volume);

    UFUNCTION()
    void UnregisterSpawnVolume(AActor* DestroyedActor);

    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    TArray<FWaveConfig> WaveConfigs;
    float SpawnTimer;

    void LoadWaveConfigs();
    void StartWave(int32 WaveIndex);

    void EndWave();
    void TrySpawnItems();

    bool IsWaveActive() const;
    ASpawnVolume* GetRandomVolume() const;
    const FItemData* GetRandomItemData(const TArray<FName>& RowNames) const;

    void PlayerNarrowSight();
};