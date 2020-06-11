// Fill out your copyright notice in the Description page of Project Settings.

#include "MenuLevelScriptActor.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "../Widget/MenuWidget.h"

AMenuLevelScriptActor::AMenuLevelScriptActor()
{
    // Class finder must be done in the constructor to make sure cooking project works fine
    static ConstructorHelpers::FClassFinder<UUserWidget> MenuWidgetClassFinder(TEXT("/Game/Source/UI/UI_MainMenu"));
    MenuWidgetClass = MenuWidgetClassFinder.Class;
}

void AMenuLevelScriptActor::BeginPlay()
{
    Super::BeginPlay();

    APlayerController *PlayerController = GetWorld()->GetFirstPlayerController();
    if (!PlayerController)
    {
        UE_LOG(LogTemp, Error, TEXT("%s: Failed to find First Player Controller!"), *GetOwner()->GetName());
        return;
    }

    MenuWidget = CreateWidget<UMenuWidget>(PlayerController, MenuWidgetClass);
    if (!MenuWidget)
    {
        UE_LOG(LogTemp, Error, TEXT("%s: Failed to find Menu Widget!"), *GetOwner()->GetName());
        return;
    }
    MenuWidget->Setup();
}
