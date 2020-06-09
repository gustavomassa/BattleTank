// Copyright Gustavo Massaneiro

#include "MortarPawn.h"
#include "../RotateComponent.h"
#include "../BarrelComponent.h"
#include "../AimingComponent.h"

// Sets default values
AMortarPawn::AMortarPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called to bind functionality to input
void AMortarPawn::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

UAimingComponent *AMortarPawn::GetTankAimingComponent() const
{
	auto TankAimingComponent = FindComponentByClass<UAimingComponent>();
	if (!TankAimingComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("%s: Failed to find Tank Aiming Component"), *GetName());
		return nullptr;
	}
	return TankAimingComponent;
}

URotateComponent *AMortarPawn::GetTurretComponent() const
{
	auto TurretComponent = FindComponentByClass<URotateComponent>();
	if (!TurretComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("%s: Failed to find Mortar Turret Component"), *GetName());
		return nullptr;
	}
	return TurretComponent;
}

UBarrelComponent *AMortarPawn::GetBarrelComponent() const
{
	auto BarrelComponent = FindComponentByClass<UBarrelComponent>();
	if (!BarrelComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("%s: Failed to find Mortar Barrel Component"), *GetName());
		return nullptr;
	}
	return BarrelComponent;
}