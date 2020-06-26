// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "BattleTankGameModeBase.h"
#include "Tank/Tank.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

void ABattleTankGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    TSubclassOf<ATank> ClassToFind = ATank::StaticClass();
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ClassToFind, EnemyTankList);
    PlayerTank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
}

void ABattleTankGameModeBase::HandleGameStart()
{
}

void ABattleTankGameModeBase::HandleGameOver(bool PlayerWon)
{
}

void ABattleTankGameModeBase::ActorDied(AActor *DeadActor)
{
}

void ABattleTankGameModeBase::GameStart()
{
}

void ABattleTankGameModeBase::GameOver(bool PlayerWon)
{
}
