// Copyright Gustavo Massaneiro

#include "MortarPawn.h"
#include "../Tank/TankAimingComponent.h"
#include "../RotateComponent.h"
#include "../BarrelComponent.h"

// Sets default values
AMortarPawn::AMortarPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	TankAimingComponent = CreateDefaultSubobject<UTankAimingComponent>(FName("Aiming Component"));
}

// Called when the game starts or when spawned
void AMortarPawn::BeginPlay()
{
	Super::BeginPlay();

	FindTurretComponent();
	FindBarrelComponent();
}

// Called to bind functionality to input
void AMortarPawn::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

UTankAimingComponent *AMortarPawn::GetTankAimingComponent() const
{
	return TankAimingComponent;
}

URotateComponent *AMortarPawn::GetTurretComponent() const
{
	return TurretComponent;
}

UBarrelComponent *AMortarPawn::GetBarrelComponent() const
{
	return BarrelComponent;
}

void AMortarPawn::FindTurretComponent()
{
	TurretComponent = FindComponentByClass<URotateComponent>();
	if (!TurretComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("%s: Failed to find Mortar Turret Component"), *GetName());
	}
}

void AMortarPawn::FindBarrelComponent()
{
	BarrelComponent = FindComponentByClass<UBarrelComponent>();
	if (!BarrelComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("%s: Failed to find Mortar Barrel Component"), *GetName());
	}
}
