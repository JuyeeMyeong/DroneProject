// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DroneDataTables.h"
#include "BaseItem.generated.h"

class ADronePlayer;
class UStaticMeshComponent;

UCLASS()
class DRONEPROJECT_API ABaseItem : public AActor
{
    GENERATED_BODY()

public:
    ABaseItem();

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    void OnItemOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
    void InitializeItem(const FItemData& InItemData);

    void ActivateEffect(ADronePlayer* Player);

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
    FItemData ItemData;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
    UStaticMeshComponent* MeshComponent;
    
};
