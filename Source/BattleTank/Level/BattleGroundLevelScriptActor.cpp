// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleGroundLevelScriptActor.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "../Widget/PlayerWidget.h"

ABattleGroundLevelScriptActor::ABattleGroundLevelScriptActor()
{
    // Class finder must be done in the constructor to make sure cooking project works fine
    static ConstructorHelpers::FClassFinder<UUserWidget> PlayerWidgetClassFinder(TEXT("/Game/Source/UI/UI_Player"));
    PlayerWidgetClass = PlayerWidgetClassFinder.Class;
}

void ABattleGroundLevelScriptActor::BeginPlay()
{
    APlayerController *PlayerController = GetWorld()->GetFirstPlayerController();
    if (!PlayerController)
    {
        UE_LOG(LogTemp, Error, TEXT("%s: Failed to find First Player Controller!"), *GetOwner()->GetName());
        return;
    }

    PlayerWidget = CreateWidget<UPlayerWidget>(PlayerController, PlayerWidgetClass);
    if (!PlayerWidget)
    {
        UE_LOG(LogTemp, Error, TEXT("%s: Failed to find Player Widget!"), *GetOwner()->GetName());
        return;
    }
    PlayerWidget->Setup();
}
