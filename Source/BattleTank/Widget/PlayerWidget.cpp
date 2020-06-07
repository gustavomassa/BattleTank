// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerWidget.h"
#include "../Tank/TankPlayerController.h"
#include "Components/Image.h"

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

    /*     if (Crosshair)
    {
        //CurrentCrossbowFiringState = EFiringState::Aiming;
        UpdateFiringStateCrosshairColor(PlayerController->GetFiringState());
        //Crosshair->ColorAndOpacityDelegate.BindUFunction(this, FName("Tocson"));
        //Crosshair->SynchronizeProperties();
        //Crosshair->SetColorAndOpacity();
    } */
}

void UPlayerWidget::UpdateCrosshairColor(const FLinearColor &LinearColor)
{
    Crosshair->SetColorAndOpacity(LinearColor);
}
