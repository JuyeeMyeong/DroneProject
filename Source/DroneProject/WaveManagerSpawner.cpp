// Fill out your copyright notice in the Description page of Project Settings.


#include "WaveManagerSpawner.h"
#include "WaveManagerComponent.h"

AWaveManagerSpawner::AWaveManagerSpawner()
{
	PrimaryActorTick.bCanEverTick = false;

	WaveManager = CreateDefaultSubobject<UWaveManagerComponent>(TEXT("WaveManager"));
}