// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerWidget.h"
#include "../Tank/TankPlayerController.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

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

void UPlayerWidget::UpdateAmmoText(const FText &Text)
{
    AmmoText->SetText(Text);
}

void UPlayerWidget::UpdateHealthBarPercent(float Percent)
{
    HealthBar->SetPercent(Percent);
}

void UPlayerWidget::BindHealthBarPercentage(UObject *InObject, const FName &InFunctionName)
{
    HealthBar->PercentDelegate.BindUFunction(InObject, InFunctionName);
    HealthBar->SynchronizeProperties();
}