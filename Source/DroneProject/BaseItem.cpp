// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseItem.h"
#include "Components/StaticMeshComponent.h"
#include "DronePlayer.h"
#include "DroneMovementComponent.h"
#include "HealthComponent.h"
#include "ScoreComponent.h"


ABaseItem::ABaseItem()
{
    PrimaryActorTick.bCanEverTick = false;

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    SetRootComponent(MeshComponent);
    MeshComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    MeshComponent->SetGenerateOverlapEvents(true);
}


void ABaseItem::BeginPlay()
{
    Super::BeginPlay();

    MeshComponent->OnComponentBeginOverlap.AddDynamic(this, &ABaseItem::OnItemOverlap);
}

void ABaseItem::OnItemOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor)
    {
        ADronePlayer* Player = Cast<ADronePlayer>(OtherActor);
        if (Player)
        {
            ActivateEffect(Player);
        }
    }
}

void ABaseItem::InitializeItem(const FItemData& InItemData)
{
    ItemData = InItemData;

    if (ItemData.Mesh)
    {
        MeshComponent->SetStaticMesh(ItemData.Mesh);
        MeshComponent->SetRelativeScale3D(FVector(ItemData.SizeScalar));
    }
}

void ABaseItem::ActivateEffect(ADronePlayer* Player)
{
    if (!Player) return;

    if (ItemData.ItemID == "HealthPotion")
    {
        if (UHealthComponent* HealthComp = Player->FindComponentByClass<UHealthComponent>())
        {
            HealthComp->Heal(1.0f);
        }
    }
    else if (ItemData.ItemID == "Bomb")
    {
        if (UHealthComponent* HealthComp = Player->FindComponentByClass<UHealthComponent>())
        {
            HealthComp->ApplyDamage(1.0f);
        }
    }
    else if (ItemData.ItemID == "ScoreCoin")
    {
        if (UScoreComponent* ScoreComp = Player->FindComponentByClass<UScoreComponent>())
        {
            ScoreComp->AddScore(1);
        }
    }
    else if (ItemData.ItemID == "Slow")
    {
        if (UDroneMovementComponent* MoveComp = Player->FindComponentByClass<UDroneMovementComponent>())
        {
            MoveComp->SetVelocityForDuration();
        }
    }
    else if (ItemData.ItemID == "InvertControl")
    {
        Player->InvertControls(ItemData.Duration);
    }

    if (ItemData.Icon && ItemData.Duration > 0.f)
    {
        Player->OnItemUsed.Broadcast(ItemData.Icon, ItemData.Duration, ItemData.Description);
    }

    Destroy();
}
