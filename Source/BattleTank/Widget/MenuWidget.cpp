// Fill out your copyright notice in the Description page of Project Settings.

#include "MenuWidget.h"
#include "../Tank/TankPlayerController.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UMenuWidget::Setup()
{
    this->AddToViewport();
    FInputModeUIOnly InputModeData;
    //InputModeData.SetWidgetToFocus(this->TakeWidget());
    InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

    ATankPlayerController *PlayerController = Cast<ATankPlayerController>(GetWorld()->GetFirstPlayerController());
    PlayerController->SetMainMenuWidgetReference(this);
    PlayerController->SetInputMode(InputModeData);
    PlayerController->bShowMouseCursor = true;

    StartGameButton->OnClicked.AddDynamic(this, &UMenuWidget::OnStartGameButtonClicked);
    QuitGameButton->OnClicked.AddDynamic(this, &UMenuWidget::OnQuitGameButtonClicked);
}

void UMenuWidget::OnStartGameButtonClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("OnStartGameButtonClicked"));

    UGameplayStatics::OpenLevel(GetWorld(), FName("BattleGround"));
}

//TODO: Implement Options Button Logic
void UMenuWidget::OnOptionsGameButtonClicked()
{
}

void UMenuWidget::OnQuitGameButtonClicked()
{
    UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, false);
}
