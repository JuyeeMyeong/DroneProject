// Fill out your copyright notice in the Description page of Project Settings.

#include "WaveManagerComponent.h"
#include "SpawnVolume.h"
#include "BaseItem.h"
#include "DronePlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/World.h"
#include "Engine/DataTable.h"
#include "Kismet/GameplayStatics.h"

UWaveManagerComponent::UWaveManagerComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    bAutoActivate = true;
    CurrentWaveIndex = 0;
    WaveElapsedTime = 0.f;
    SpawnTimer = 0.f;
}
 
void UWaveManagerComponent::BeginPlay()
{
    Super::BeginPlay();

    LoadWaveConfigs();

    TArray<AActor*> Found;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), Found);
    for (AActor* Actor : Found)
        if (auto* Vol = Cast<ASpawnVolume>(Actor))
        {
            RegisterSpawnVolume(Vol);
        }

    if (WaveConfigs.Num() > 0)
    {
        StartWave(1);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("No wave configs found!"));
    }
}

void UWaveManagerComponent::RegisterSpawnVolume(ASpawnVolume* Volume)
{
    if (Volume && !SpawnVolumes.Contains(Volume))
    {
        SpawnVolumes.Add(Volume);
        Volume->OnDestroyed.AddDynamic(this, &UWaveManagerComponent::UnregisterSpawnVolume);
    }
}

void UWaveManagerComponent::UnregisterSpawnVolume(AActor* DestroyedActor)
{
    if (ASpawnVolume* Volume = Cast<ASpawnVolume>(DestroyedActor))
    {
        SpawnVolumes.Remove(Volume);
    }
}

void UWaveManagerComponent::LoadWaveConfigs()
{
    WaveConfigs.Empty();
    if (!WaveConfigTable) return;
    for (auto& Row : WaveConfigTable->GetRowMap())
    {
        if (FWaveConfig* Config = (FWaveConfig*)Row.Value)
        {
            WaveConfigs.Add(*Config);
        }
    }
    WaveConfigs.Sort([](const FWaveConfig& A, const FWaveConfig& B) {
        return A.WaveIndex < B.WaveIndex;
        });
}

void UWaveManagerComponent::StartWave(int32 WaveIndex)
{
    CurrentWaveIndex = WaveIndex;
    WaveElapsedTime = 0.f;
    SpawnTimer = 0.f;
    OnWaveStarted.Broadcast(WaveIndex, WaveConfigs[WaveIndex - 1].TimeLimit);
    OnWaveStarted.Broadcast(WaveIndex, WaveConfigs[WaveIndex - 1].TimeLimit);

    if (WaveIndex == 3)
    {
        PlayerNarrowSight();
    }

    FString Message = FString::Printf(TEXT("Wave %d START!"), WaveIndex);
    UE_LOG(LogTemp, Log, TEXT("%s"), *Message);

    TrySpawnItems();
}

void UWaveManagerComponent::EndWave()
{
    TArray<AActor*> FoundItems;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseItem::StaticClass(), FoundItems);

    for (AActor* Actor : FoundItems)
    {
        if (Actor)
        {
            Actor->Destroy();
        }
    }

    int32 Next = CurrentWaveIndex + 1;
    if (WaveConfigs.IndexOfByPredicate([Next](const FWaveConfig& C) { return C.WaveIndex == Next; }) != INDEX_NONE)
    {
        StartWave(Next);
    }
    else
    {
        OnWaveStarted.Broadcast(0, 0.f);
    }
}

void UWaveManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (!IsWaveActive()) return;

    WaveElapsedTime += DeltaTime;

    if (WaveElapsedTime >= WaveConfigs[CurrentWaveIndex - 1].TimeLimit)
    {
        EndWave();
    }
}


void UWaveManagerComponent::TrySpawnItems()
{
    if (!IsWaveActive() || !ItemDataTable) return;

    FWaveConfig& Config = WaveConfigs[CurrentWaveIndex - 1];

    const TArray<FName> RowNames = ItemDataTable->GetRowNames();
    if (RowNames.Num() == 0) return;

    TArray<FName> ValidRowNames;
    ValidRowNames.Reserve(RowNames.Num());

    for (const FName& RowName : RowNames)
    {
        const FItemData* Data = ItemDataTable->FindRow<FItemData>(RowName, TEXT("TrySpawnItems"));
        if (!Data) continue;
        if (CurrentWaveIndex < 2 && Data->ItemID == FName("Lightning")) continue;
        ValidRowNames.Add(RowName);
    }

    if (ValidRowNames.IsEmpty()) return;

    for (const FSpawnCount& SC : Config.SpawnCounts)
    {
        if (SC.Count <= 0 || !*SC.ItemClass) continue;

        for (int32 i = 0; i < SC.Count; ++i)
        {
            if (ASpawnVolume* Volume = GetRandomVolume())
                if (const FItemData* InitData = GetRandomItemData(ValidRowNames))
                {

                    UClass* Raw = SC.ItemClass.Get();
                    if (Raw && Raw->IsChildOf(ABaseItem::StaticClass()))
                    {
                        TSubclassOf<ABaseItem> ItemSubclass = Raw;

                        if (ABaseItem* NewItem = Volume->SpawnItem(ItemSubclass))
                        {
                            NewItem->InitializeItem(*InitData);
                        }
                    }
                }
        }
    }
}

bool UWaveManagerComponent::IsWaveActive() const
{
    return CurrentWaveIndex > 0
        && WaveConfigs.IsValidIndex(CurrentWaveIndex - 1)
        && !SpawnVolumes.IsEmpty();
}

ASpawnVolume* UWaveManagerComponent::GetRandomVolume() const
{
    if (SpawnVolumes.IsEmpty())
        return nullptr;

    int32 Idx = FMath::RandRange(0, SpawnVolumes.Num() - 1);
    return SpawnVolumes[Idx];
}

const FItemData* UWaveManagerComponent::GetRandomItemData(const TArray<FName>& RowNames) const
{
    if (RowNames.IsEmpty() || !ItemDataTable)
        return nullptr;

    int32 Idx = FMath::RandRange(0, RowNames.Num() - 1);
    return ItemDataTable->FindRow<FItemData>(RowNames[Idx], TEXT("TrySpawnItems"));
}

void UWaveManagerComponent::PlayerNarrowSight()
{
    if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
    {
        if (ADronePlayer* Drone = Cast<ADronePlayer>(PC->GetPawn()))
        {
            if (USpringArmComponent* Boom = Drone->FindComponentByClass<USpringArmComponent>())
            {
                Boom->TargetArmLength = 150.0f;
            }
        }
    }
}


