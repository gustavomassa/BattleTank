// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"
#include "GameFramework/SpringArmComponent.h"
#include "TankBodyComponent.h"
#include "TankTurretComponent.h"
#include "TankBarrelComponent.h"
#include "TankTrackComponent.h"
#include "TankMovementComponent.h"
#include "TankAimingComponent.h"
#include "TankProjectile.h"

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

	TankBodyComponent->SetupPhysics();
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
	TankTurretComponent = FindComponentByClass<UTankTurretComponent>();
	if (!TankTurretComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("%s: Failed to find Tank Turret Component"), *GetOwner()->GetName());
	}
	SetTankTurretReference(TankTurretComponent);
}

void ATank::FindBarrelComponent()
{
	TankBarrelComponent = FindComponentByClass<UTankBarrelComponent>();
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

void ATank::SetTankTrackLeftReference(UTankTrackComponent *TrackLeftToSet)
{
	TankTrackLeftComponent = TrackLeftToSet;
	TankMovementComponent->SetTankTrackLeftReference(TrackLeftToSet);
}

void ATank::SetTankTrackRightReference(UTankTrackComponent *TrackRightToSet)
{
	TankTrackRightComponent = TrackRightToSet;
	TankMovementComponent->SetTankTrackRightReference(TrackRightToSet);
}

USpringArmComponent *ATank::GetCameraComponent() const
{
	return TankCameraComponent;
}

const UTankMovementComponent *ATank::GetTankMovementComponent() const
{
	return TankMovementComponent;
}

const UTankBodyComponent *ATank::GetBodyComponent() const
{
	return TankBodyComponent;
}

const UTankTurretComponent *ATank::GetTurretComponent() const
{
	return TankTurretComponent;
}

const UTankBarrelComponent *ATank::GetBarrelComponent() const
{
	return TankBarrelComponent;
}

const UTankTrackComponent *ATank::GetTankTrackLeftComponent() const
{
	return TankTrackLeftComponent;
}

const UTankTrackComponent *ATank::GetTankTrackRightComponent() const
{
	return TankTrackRightComponent;
}

float ATank::GetMass() const
{
	return Mass;
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

float ATank::GetTurretMaxDegressPerSecond() const
{
	return TurretMaxDegressPerSecond;
}

float ATank::GetBarrelMaxDegressPerSecond() const
{
	return BarrelMaxDegressPerSecond;
}

float ATank::GetBarrelMinElevationDegress() const
{
	return BarrelMinElevationDegress;
}

float ATank::GetBarrelMaxElevationDegress() const
{
	return BarrelMaxElevationDegress;
}

float ATank::GetProjectileLaunchSpeed() const
{
	return ProjectileLaunchSpeed;
}

bool ATank::AimAt(FVector &HitLocation, FVector &Out_AimDirection)
{
	return TankAimingComponent->AimAt(HitLocation, Out_AimDirection);
}

void ATank::Aim(FVector &TargetLocation)
{
	TankAimingComponent->Aim(TargetLocation);
}

void ATank::Fire()
{
	UE_LOG(LogTemp, Warning, TEXT("Fire!"));

	auto Projectile = GetWorld()->SpawnActor<ATankProjectile>(
		TankProjectile,
		TankBarrelComponent->GetProjectileLaunchLocation(),
		TankBarrelComponent->GetProjectileLaunchRotation());
	if (Projectile)
	{
		Projectile->Launch(ProjectileLaunchSpeed);
	}
}