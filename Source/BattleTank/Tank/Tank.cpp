// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"
#include "GameFramework/SpringArmComponent.h"
#include "TankTurretComponent.h"
#include "TankBarrelComponent.h"
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

	// Find Components
	TankCameraComponent = FindComponentByClass<USpringArmComponent>();
	if (!TankCameraComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("%s: Failed to find Tank Camera Component"), *GetOwner()->GetName());
	}
	SetTankCameraReference(TankCameraComponent);

	TankTurretComponent = FindComponentByClass<UTankTurretComponent>();
	if (!TankTurretComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("%s: Failed to find Tank Turret Component"), *GetOwner()->GetName());
	}
	SetTankTurretReference(TankTurretComponent);

	TankBarrelComponent = FindComponentByClass<UTankBarrelComponent>();
	if (!TankBarrelComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("%s: Failed to find Tank Barrel Component"), *GetOwner()->GetName());
	}
	SetTankBarrelReference(TankBarrelComponent);
}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

// TODO: Create a TankCameraComponent
void ATank::SetTankCameraReference(USpringArmComponent *CameraToSet)
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

USpringArmComponent *ATank::GetCameraComponent() const
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