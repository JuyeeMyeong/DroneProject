// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ScoreComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScoreChanged, int32, CurrentScore);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DRONEPROJECT_API UScoreComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UScoreComponent();

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Score")
    int32 CurrentScore;

    UPROPERTY(BlueprintAssignable, Category = "Score")
    FOnScoreChanged OnScoreChanged;

    UFUNCTION(BlueprintCallable, Category = "Score")
    void AddScore(int32 Amount);

    //UFUNCTION(BlueprintCallable, Category = "Score")
    //void ResetScore();

protected:
    virtual void BeginPlay() override;
};