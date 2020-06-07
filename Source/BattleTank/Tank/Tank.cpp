// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"
#include "GameFramework/SpringArmComponent.h"
#include "TankBodyComponent.h"
#include "../RotateComponent.h"
#include "../BarrelComponent.h"
#include "TankTrackComponent.h"
#include "TankMovementComponent.h"
#include "TankAimingComponent.h"

// Sets default values
ATank::ATank()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Register Components
	TankMovementComponent = CreateDefaultSubobject<UTankMovementComponent>(FName("Movement Component"));
	TankAimingComponent = CreateDefaultSubobject<UTankAimingComponent>(FName("Aiming Component"));
}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();

	// Find Components
	FindCameraComponent();
	FindBodyComponent();
	FindTurretComponent();
	FindBarrelComponent();
	FindTankTrackComponents();

	// Setup Components Physics
	TankBodyComponent->SetupPhysics();
	TankTurretComponent->SetupPhysics();
	TankBarrelComponent->SetupPhysics();
	TankTrackLeftComponent->SetupPhysics();
	TankTrackRightComponent->SetupPhysics();
}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ATank::FindCameraComponent()
{
	TankCameraComponent = FindComponentByClass<USpringArmComponent>();
	if (!TankCameraComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("%s: Failed to find Tank Camera Component"), *GetOwner()->GetName());
	}
	SetTankCameraReference(TankCameraComponent);
}

void ATank::FindBodyComponent()
{
	TankBodyComponent = FindComponentByClass<UTankBodyComponent>();
	if (!TankBodyComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("%s: Failed to find Tank Body Component"), *GetOwner()->GetName());
	}
	SetTankBodyReference(TankBodyComponent);
}

void ATank::FindTurretComponent()
{
	TankTurretComponent = FindComponentByClass<URotateComponent>();
	if (!TankTurretComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("%s: Failed to find Tank Turret Component"), *GetOwner()->GetName());
	}
	SetTankTurretReference(TankTurretComponent);
}

void ATank::FindBarrelComponent()
{
	TankBarrelComponent = FindComponentByClass<UBarrelComponent>();
	if (!TankBarrelComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("%s: Failed to find Tank Barrel Component"), *GetOwner()->GetName());
	}
	SetTankBarrelReference(TankBarrelComponent);
}

void ATank::FindTankTrackComponents()
{
	TArray<UTankTrackComponent *> TankTrackComponents;
	GetComponents<UTankTrackComponent>(TankTrackComponents, false);

	if (TankTrackComponents.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("%s: Failed to find Tank Track Components"), *GetOwner()->GetName());
		return;
	}
	for (auto TankTrackComponent : TankTrackComponents)
	{
		if (TankTrackComponent)
		{
			if (TankTrackComponent->GetName() == "TrackLeft")
			{
				SetTankTrackLeftReference(TankTrackComponent);
			}
			else if (TankTrackComponent->GetName() == "TrackRight")
			{
				SetTankTrackRightReference(TankTrackComponent);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("%s: Failed to find Tank Track Components by Name"), *GetOwner()->GetName());
			}
		}
	}
}

// TODO: Create a TankCameraComponent
void ATank::SetTankCameraReference(USpringArmComponent *CameraToSet)
{
	TankCameraComponent = CameraToSet;
}

void ATank::SetTankBodyReference(UTankBodyComponent *BodyToSet)
{
	TankBodyComponent = BodyToSet;
}

void ATank::SetTankTurretReference(URotateComponent *TurretToSet)
{
	TankTurretComponent = TurretToSet;
}

void ATank::SetTankBarrelReference(UBarrelComponent *BarrelToSet)
{
	TankBarrelComponent = BarrelToSet;
}

void ATank::SetTankTrackLeftReference(UTankTrackComponent *TrackLeftToSet)
{
	TankTrackLeftComponent = TrackLeftToSet;
}

void ATank::SetTankTrackRightReference(UTankTrackComponent *TrackRightToSet)
{
	TankTrackRightComponent = TrackRightToSet;
}

USpringArmComponent *ATank::GetCameraComponent() const
{
	return TankCameraComponent;
}

const UTankMovementComponent *ATank::GetTankMovementComponent() const
{
	return TankMovementComponent;
}

UTankAimingComponent *ATank::GetTankAimingComponent() const
{
	return TankAimingComponent;
}

const UTankBodyComponent *ATank::GetBodyComponent() const
{
	return TankBodyComponent;
}

URotateComponent *ATank::GetTurretComponent() const
{
	return TankTurretComponent;
}

UBarrelComponent *ATank::GetBarrelComponent() const
{
	return TankBarrelComponent;
}

UTankTrackComponent *ATank::GetTankTrackLeftComponent() const
{
	return TankTrackLeftComponent;
}

UTankTrackComponent *ATank::GetTankTrackRightComponent() const
{
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