// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"
#include "TankAimingComponent.h"

// Sets default values
ATank::ATank()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Register Components
	TankAimingComponent = CreateDefaultSubobject<UTankAimingComponent>(FName("Aiming Component"));
}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();
}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

// TODO: Create a TankCameraComponent
void ATank::SetTankCameraReference(USceneComponent *CameraToSet)
{
	TankCameraComponent = CameraToSet;
}

void ATank::SetTankTurretReference(UTankTurretComponent *TurretToSet)
{
	TankTurretComponent = TurretToSet;
	TankAimingComponent->SetTankTurretReference(TurretToSet);
}

void ATank::SetTankBarrelReference(UTankBarrelComponent *BarrelToSet)
{
	TankBarrelComponent = BarrelToSet;
	TankAimingComponent->SetTankBarrelReference(BarrelToSet);
}

USceneComponent *ATank::GetCameraComponent() const
{
	return TankCameraComponent;
}

UTankTurretComponent *ATank::GetTurretComponent() const
{
	return TankTurretComponent;
}

UTankBarrelComponent *ATank::GetBarrelComponent() const
{
	return TankBarrelComponent;
}

void ATank::AimAt(FVector &HitLocation)
{
	TankAimingComponent->AimAt(HitLocation);
}

void ATank::Aim(FVector &TargetLocation)
{
	TankAimingComponent->Aim(TargetLocation);
}

void ATank::OnFire()
{
	UE_LOG(LogTemp, Warning, TEXT("Fire!"));
}