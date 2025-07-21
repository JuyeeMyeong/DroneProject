// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreComponent.h"

UScoreComponent::UScoreComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    CurrentScore = 0;
}

void UScoreComponent::BeginPlay()
{
    Super::BeginPlay();
    CurrentScore = 0;
}

void UScoreComponent::AddScore(int32 Amount)
{
    if (Amount == 0)
    {
        return;
    }
    CurrentScore += Amount;
    OnScoreChanged.Broadcast(CurrentScore);
}

/* 점수 리셋해버리는 아이템 (?)
void UScoreComponent::ResetScore()
{
    CurrentScore = 0;
    OnScoreChanged.Broadcast(CurrentScore);
}*/ 


