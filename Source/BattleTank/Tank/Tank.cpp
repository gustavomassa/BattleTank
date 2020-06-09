// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"
#include "GameFramework/SpringArmComponent.h"
#include "TankBodyComponent.h"
#include "../RotateComponent.h"
#include "../BarrelComponent.h"
#include "../AimingComponent.h"
#include "TankTrackComponent.h"
#include "TankMovementComponent.h"

// Sets default values
ATank::ATank()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Register Components
	TankMovementComponent = CreateDefaultSubobject<UTankMovementComponent>(FName("Movement Component"));
	//TankAimingComponent = CreateDefaultSubobject<UTankAimingComponent>(FName("Aiming Component"));
}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

USpringArmComponent *ATank::GetCameraComponent() const
{
	auto TankCameraComponent = FindComponentByClass<USpringArmComponent>();
	if (!TankCameraComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("%s: Failed to find Tank Camera Component"), *GetOwner()->GetName());
		return nullptr;
	}
	return TankCameraComponent;
}

const UTankMovementComponent *ATank::GetTankMovementComponent() const
{
	return TankMovementComponent;
}

UAimingComponent *ATank::GetTankAimingComponent() const
{
	auto TankAimingComponent = FindComponentByClass<UAimingComponent>();
	if (!TankAimingComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("%s: Failed to find Tank Aiming Component"), *GetOwner()->GetName());
		return nullptr;
	}
	return TankAimingComponent;
}

const UTankBodyComponent *ATank::GetBodyComponent() const
{
	auto TankBodyComponent = FindComponentByClass<UTankBodyComponent>();
	if (!TankBodyComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("%s: Failed to find Tank Body Component"), *GetOwner()->GetName());
		return nullptr;
	}
	return TankBodyComponent;
}

URotateComponent *ATank::GetTurretComponent() const
{
	auto TankTurretComponent = FindComponentByClass<URotateComponent>();
	if (!TankTurretComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("%s: Failed to find Tank Turret Component"), *GetOwner()->GetName());
		return nullptr;
	}
	return TankTurretComponent;
}

UBarrelComponent *ATank::GetBarrelComponent() const
{
	auto TankBarrelComponent = FindComponentByClass<UBarrelComponent>();
	if (!TankBarrelComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("%s: Failed to find Tank Barrel Component"), *GetOwner()->GetName());
		return nullptr;
	}
	return TankBarrelComponent;
}

UTankTrackComponent *ATank::GetTankTrackLeftComponent() const
{
	TArray<UTankTrackComponent *> TankTrackComponents;
	GetComponents<UTankTrackComponent>(TankTrackComponents, false);

	if (TankTrackComponents.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("%s: Failed to find Tank Track Components"), *GetOwner()->GetName());
		return nullptr;
	}

	UTankTrackComponent *TankTrackLeftComponent = nullptr;

	for (auto TankTrackComponent : TankTrackComponents)
	{
		if (TankTrackComponent)
		{
			if (TankTrackComponent->GetName() == "TrackLeft")
			{
				TankTrackLeftComponent = TankTrackComponent;
				break;
			}
		}
	}
	if (!TankTrackLeftComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("%s: Failed to find Tank Track Left Component"), *GetOwner()->GetName());
		return nullptr;
	}

	return TankTrackLeftComponent;
}

UTankTrackComponent *ATank::GetTankTrackRightComponent() const
{
	TArray<UTankTrackComponent *> TankTrackComponents;
	GetComponents<UTankTrackComponent>(TankTrackComponents, false);

	if (TankTrackComponents.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("%s: Failed to find Tank Track Components"), *GetOwner()->GetName());
		return nullptr;
	}

	UTankTrackComponent *TankTrackRightComponent = nullptr;

	for (auto TankTrackComponent : TankTrackComponents)
	{
		if (TankTrackComponent)
		{
			if (TankTrackComponent->GetName() == "TrackRight")
			{
				TankTrackRightComponent = TankTrackComponent;
				break;
			}
		}
	}
	if (!TankTrackRightComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("%s: Failed to find Tank Track Right Component"), *GetOwner()->GetName());
		return nullptr;
	}

	return TankTrackRightComponent;
}

float ATank::GetDefaultMass() const
{
	return Mass;
}

float ATank::GetMass() const
{
	return Cast<UPrimitiveComponent>(GetRootComponent())->GetMass();
}

TSubclassOf<ATankProjectile> ATank::GetTankProjectile() const
{
	return TankProjectile;
}

float ATank::GetReloadTimeInSeconds() const
{
	return ReloadTimeInSeconds;
}

float ATank::GetGravityAcceleration() const
{
	return GravityAcceleration;
}

float ATank::GetForceAdjustment() const
{
	return ForceAdjustment;
}

float ATank::GetProjectileLaunchSpeed() const
{
	return ProjectileLaunchSpeed;
}