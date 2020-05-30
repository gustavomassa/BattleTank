// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "BattleGroundLevelScriptActor.generated.h"

//Forward Declarations
class UPlayerWidget;

/**
 * 
 */
UCLASS()
class BATTLETANK_API ABattleGroundLevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()

public:
	ABattleGroundLevelScriptActor();
	virtual void BeginPlay() override;

private:
	TSubclassOf<UUserWidget> PlayerWidgetClass;
	UPlayerWidget *PlayerWidget{nullptr};
};
