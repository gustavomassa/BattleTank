// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "Tank.h"
#include "../AimingComponent.h"
#include "Engine/World.h"

void ATankAIController::BeginPlay()
{
    Super::BeginPlay();

    PlayerTank = Cast<ATank>(GetWorld()->GetFirstPlayerController()->GetPawn());
    if (!ensure(PlayerTank))
    {
        UE_LOG(LogTemp, Error, TEXT("AIController: Failed to get Player Tank"));
    }
    UE_LOG(LogTemp, Warning, TEXT("AIController: Player Tank %s"), *PlayerTank->GetName());
}

void ATankAIController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (PlayerTank)
    {
        // Move towards the player
        auto moved = MoveToActor(PlayerTank, AcceptanceRadius);
        //EPathFollowingRequestResult::Type

        // Aim towards the player
        PlayerTankLocation = PlayerTank->GetActorLocation();
        // We don't need to update the crosshair color for AI
        auto AimingComponent = ControlledTank->GetTankAimingComponent();
        if (AimingComponent)
        {
            AimingComponent->AimAt(PlayerTankLocation);
            if (AimingComponent->GetFiringState() == EFiringState::Locked)
            {
                AimingComponent->Fire();
            }
        }
    }
}

void ATankAIController::SetPawn(APawn *InPawn)
{
    Super::SetPawn(InPawn);

    if (InPawn)
    {
        ControlledTank = Cast<ATank>(InPawn);
        if (!ensure(ControlledTank))
        {
            UE_LOG(LogTemp, Error, TEXT("AIController: Failed to get Controlled Tank"));
        }
        UE_LOG(LogTemp, Warning, TEXT("AIController: Controlled Tank %s"), *ControlledTank->GetName());

        // Subscribe to Events
        ControlledTank->OnDeath.AddUniqueDynamic(this, &ATankAIController::OnTankDeath);
    }
}

void ATankAIController::OnTankDeath()
{
    if (ControlledTank)
    {
        ControlledTank->DetachFromControllerPendingDestroy();
        UE_LOG(LogTemp, Warning, TEXT("ATankAIController OnTankDeath"));
    }
}