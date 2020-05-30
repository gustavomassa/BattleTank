// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerWidget.h"
#include "../Tank/TankPlayerController.h"

void UPlayerWidget::Setup()
{
    this->AddToViewport();
    FInputModeGameOnly InputModeData;
    /*     FInputModeGameAndUI InputModeData;
    InputModeData.SetWidgetToFocus(this->TakeWidget());
    InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock); */

    ATankPlayerController *PlayerController = Cast<ATankPlayerController>(GetWorld()->GetFirstPlayerController());
    PlayerController->SetPlayerWidgetReference(this);
    PlayerController->SetInputMode(InputModeData);
    PlayerController->bShowMouseCursor = false;
}
