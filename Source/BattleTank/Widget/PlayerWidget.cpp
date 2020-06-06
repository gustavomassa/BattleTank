// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerWidget.h"
#include "../Tank/TankPlayerController.h"
#include "../Tank/TankAimingComponent.h"
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

    if (Crosshair)
    {
        //CurrentCrossbowFiringState = EFiringState::Aiming;
        UpdateFiringStateCrosshairColor(PlayerController->GetFiringState());
        //Crosshair->ColorAndOpacityDelegate.BindUFunction(this, FName("Tocson"));
        //Crosshair->SynchronizeProperties();
        //Crosshair->SetColorAndOpacity();
    }
}

void UPlayerWidget::UpdateFiringStateCrosshairColor(const EFiringState &FiringState)
{
    // Just update crossbow if the state changed
    if (CurrentCrossbowFiringState == FiringState)
    {
        return;
    }

    FLinearColor InColorAndOpacity = Crosshair->ColorAndOpacity;

    switch (FiringState)
    {
    case EFiringState::Reloading:
        InColorAndOpacity = FLinearColor(255.0f, 0.0f, 0.0f);
        break;
    case EFiringState::Aiming:
        InColorAndOpacity = FLinearColor(255.0f, 255.0f, 0.0f);
        break;
    case EFiringState::Locked:
        InColorAndOpacity = FLinearColor(0.0f, 255.0f, 0.0f);
        break;
    }
    Crosshair->SetColorAndOpacity(InColorAndOpacity);
    CurrentCrossbowFiringState = FiringState;
}
