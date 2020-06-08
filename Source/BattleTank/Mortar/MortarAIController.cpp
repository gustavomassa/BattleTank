// Copyright Gustavo Massaneiro

#include "MortarAIController.h"
#include "MortarPawn.h"
#include "../Tank/Tank.h"
#include "../Tank/TankAimingComponent.h"
#include "Engine/World.h"

void AMortarAIController::BeginPlay()
{
    Super::BeginPlay();

    ControlledMortar = GetControlledMortar();
    if (!ensure(ControlledMortar))
    {
        UE_LOG(LogTemp, Error, TEXT("AIController: Failed to get Controlled Mortar!"));
    }
    UE_LOG(LogTemp, Warning, TEXT("%s: Controlled Mortar %s"), *GetName(), *ControlledMortar->GetName());

    PlayerTank = Cast<ATank>(GetWorld()->GetFirstPlayerController()->GetPawn());
    if (!ensure(PlayerTank))
    {
        UE_LOG(LogTemp, Error, TEXT("%s: Failed to get Player Tank"), *GetName());
    }
    UE_LOG(LogTemp, Warning, TEXT("%s: Player Tank %s"), *GetName(), *PlayerTank->GetName());
}

void AMortarAIController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (PlayerTank)
    {
        PlayerTankLocation = PlayerTank->GetActorLocation();
        auto AimingComponent = ControlledMortar->GetTankAimingComponent();
        AimingComponent->AimAt(PlayerTankLocation);

        if (AimingComponent->GetFiringState() == EFiringState::Locked)
        {
            AimingComponent->Fire();
        }
    }
}

AMortarPawn *AMortarAIController::GetControlledMortar() const
{
    return Cast<AMortarPawn>(GetPawn());
}