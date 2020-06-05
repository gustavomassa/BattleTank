// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "Tank.h"
#include "Engine/World.h"

void ATankAIController::BeginPlay()
{
    Super::BeginPlay();

    ControlledTank = GetControlledTank();
    if (!ensure(ControlledTank))
    {
        UE_LOG(LogTemp, Error, TEXT("AIController: Failed to get Controlled Tank"));
    }
    UE_LOG(LogTemp, Warning, TEXT("AIController: Controlled Tank %s"), *ControlledTank->GetName());

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
        ControlledTank->AimAt(PlayerTankLocation);

        // TODO: Don't fire every frame
        //ControlledTank->Fire();
        // Fire if ready
    }
}

ATank *ATankAIController::GetControlledTank() const
{
    // Tank is a especialization of the Pawn (Subtype - Runtime Polymorphism)
    return Cast<ATank>(GetPawn());
}