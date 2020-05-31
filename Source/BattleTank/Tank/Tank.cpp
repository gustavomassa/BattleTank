// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"
#include "GameFramework/SpringArmComponent.h"
#include "TankBodyComponent.h"
#include "TankTurretComponent.h"
#include "TankBarrelComponent.h"
#include "TankTrackComponent.h"
#include "TankAimingComponent.h"
#include "TankProjectile.h"

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

	FindTankTrackComponents();

	TankBodyComponent = FindComponentByClass<UTankBodyComponent>();
	if (!TankBodyComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("%s: Failed to find Tank Body Component"), *GetOwner()->GetName());
	}
	SetTankBodyReference(TankBodyComponent);
	TankBodyComponent->SetupPhysics();
}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
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
}

void ATank::SetTankTrackRightReference(UTankTrackComponent *TrackRightToSet)
{
	TankTrackRightComponent = TrackRightToSet;
}

USpringArmComponent *ATank::GetCameraComponent() const
{
	return TankCameraComponent;
}

UTankBodyComponent *ATank::GetBodyComponent() const
{
	return TankBodyComponent;
}

UTankTurretComponent *ATank::GetTurretComponent() const
{
	return TankTurretComponent;
}

UTankBarrelComponent *ATank::GetBarrelComponent() const
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

float ATank::GetMass() const
{
	return Mass;
}

float ATank::GetReloadTimeInSeconds() const
{
	return ReloadTimeInSeconds;
}

/* float ATank::GetProjectileLaunchSpeed() const
{
	return ProjectileLaunchSpeed;
}*/

void ATank::AimAt(FVector &HitLocation)
{
	TankAimingComponent->AimAt(HitLocation);
}

void ATank::Aim(FVector &TargetLocation)
{
	TankAimingComponent->Aim(TargetLocation);
}

void ATank::Fire()
{
	// Check if we are reloaded
	if (TankBarrelComponent && ((FPlatformTime::Seconds() - LastFireTime) > ReloadTimeInSeconds))
	{
		UE_LOG(LogTemp, Warning, TEXT("Fire!"));

		auto Projectile = GetWorld()->SpawnActor<ATankProjectile>(
			TankProjectile,
			TankBarrelComponent->GetProjectileLaunchLocation(),
			TankBarrelComponent->GetProjectileLaunchRotation());
		if (Projectile)
		{
			Projectile->Launch(TankAimingComponent->GetProjectileLaunchSpeed());
			// Reset the timer
			LastFireTime = FPlatformTime::Seconds();
		}
	}
}