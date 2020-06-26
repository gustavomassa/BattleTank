// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BattleTankGameModeBase.generated.h"

// Forward Declarations
class ATank;
/**
 * 
 */
UCLASS()
class BATTLETANK_API ABattleTankGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	void ActorDied(AActor *DeadActor);
	void GameStart();
	void GameOver(bool PlayerWon);

protected:
	virtual void BeginPlay() override;

private:
	ATank *PlayerTank{nullptr};
	TArray<AActor *> EnemyTankList;

	void HandleGameStart();
	void HandleGameOver(bool PlayerWon);
};
