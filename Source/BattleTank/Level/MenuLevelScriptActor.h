// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "MenuLevelScriptActor.generated.h"

//Forward Declarations
class UMenuWidget;

/**
 * 
 */
UCLASS()
class BATTLETANK_API AMenuLevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()

public:
	AMenuLevelScriptActor();
	virtual void BeginPlay() override;

private:
	TSubclassOf<UUserWidget> MenuWidgetClass;
	UMenuWidget *MenuWidget{nullptr};
};
