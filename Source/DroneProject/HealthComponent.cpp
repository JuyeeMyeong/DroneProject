// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthComponent.h"

UHealthComponent::UHealthComponent()
    : MaxHealth(10.0f)
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    CurrentHealth = MaxHealth;
}

void UHealthComponent::ApplyDamage(float Amount)
{
    ChangeHealth(-Amount);
}

void UHealthComponent::Heal(float Amount)
{
    ChangeHealth(Amount);
}

void UHealthComponent::ChangeHealth(float Delta)
{
    if (FMath::IsNearlyZero(Delta) || CurrentHealth <= 0.f)
        return;

    const float OldHealth = CurrentHealth;
    CurrentHealth = FMath::Clamp(OldHealth + Delta, 0.f, MaxHealth);

    const float OldPercent = OldHealth / MaxHealth;
    const float NewPercent = CurrentHealth / MaxHealth;

    OnHealthChanged.Broadcast(OldPercent, NewPercent);

    if (CurrentHealth <= 0.f)
    {
        OnDeath.Broadcast();
    }
}