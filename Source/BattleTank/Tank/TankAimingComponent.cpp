// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAimingComponent.h"
#include "Tank.h"
#include "TankTurretComponent.h"
#include "TankBarrelComponent.h"
#include "TankPlayerController.h"
#include "TankProjectile.h"
#include "../Widget/PlayerWidget.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UTankAimingComponent::BeginPlay()
{
	Super::BeginPlay();

	ControlledTank = Cast<ATank>(GetOwner());
	TankPlayerController = Cast<ATankPlayerController>(ControlledTank->GetController());

	// Init reload timer
	LastFireTime = FPlatformTime::Seconds();
	// Init AmmoCount
	AmmoCount = 10;

	// Init Player UI
	if (TankPlayerController)
	{
		TankPlayerController->GetPlayerWidget()->UpdateAmmoText(FText::FromString(FString::FromInt(AmmoCount)));
	}
}

// Called every frame
void UTankAimingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	LastFiringState = FiringState;

	if (AmmoCount <= 0)
	{
		FiringState = EFiringState::OutOfAmmo;
	}
	else if ((FPlatformTime::Seconds() - LastFireTime) < ControlledTank->GetReloadTimeInSeconds())
	{
		FiringState = EFiringState::Reloading;
	}
	else if (IsCrosshairLocked(0.02f))
	{
		FiringState = EFiringState::Locked;
	}
	else
	{
		FiringState = EFiringState::Aiming;
	}

	// AI does not need to update Widgets
	if (TankPlayerController && FiringState != LastFiringState)
	{
		TankPlayerController->GetPlayerWidget()->UpdateCrosshairColor(GetFiringStateCrosshairColor());
	}
}

bool UTankAimingComponent::IsCrosshairLocked(float Tolerance)
{
	FVector BarrelDirection = ControlledTank->GetBarrelComponent()->GetForwardVector();
	bool bValidateLockX = FMath::Abs(BarrelDirection.X - AimDirection.X) <= Tolerance;
	bool bValidateLockY = FMath::Abs(BarrelDirection.Y - AimDirection.Y) <= Tolerance;
	bool bValidateLockZ = (FMath::Abs(BarrelDirection.Z) > Tolerance) ? FMath::Abs(BarrelDirection.Z - AimDirection.Z) <= Tolerance : true;
	return (bValidateLockX && bValidateLockY && bValidateLockZ);
}

const EFiringState &UTankAimingComponent::GetFiringState() const
{
	return FiringState;
}

const uint8 &UTankAimingComponent::GetAmmoCount() const
{
	return AmmoCount;
}

const FLinearColor UTankAimingComponent::GetFiringStateCrosshairColor() const
{
	FLinearColor InColorAndOpacity;

	switch (FiringState)
	{
	case EFiringState::OutOfAmmo:
		InColorAndOpacity = FLinearColor(255.0f, 255.0f, 255.0f);
		break;
	case EFiringState::Reloading:
		InColorAndOpacity = FLinearColor(255.0f, 0.0f, 0.0f);
		break;
	case EFiringState::Aiming:
		InColorAndOpacity = FLinearColor(255.0f, 255.0f, 0.0f);
		break;
	case EFiringState::Locked:
		InColorAndOpacity = FLinearColor(0.0f, 255.0f, 0.0f);
		break;
	}
	return InColorAndOpacity;
}

void UTankAimingComponent::UpdateAmmoCount()
{
	if (AmmoCount > 0)
	{
		--AmmoCount;
		if (TankPlayerController)
		{
			TankPlayerController->GetPlayerWidget()->UpdateAmmoText(FText::FromString(FString::FromInt(AmmoCount)));
		}
	}
}

void UTankAimingComponent::MoveTurret()
{
	FRotator DeltaRotator = (AimDirection.Rotation() - ControlledTank->GetTurretComponent()->GetForwardVector().Rotation());
	ControlledTank->GetTurretComponent()->Rotate(DeltaRotator.Yaw);
}

void UTankAimingComponent::MoveBarrel()
{
	FRotator DeltaRotator = (AimDirection.Rotation() - ControlledTank->GetBarrelComponent()->GetForwardVector().Rotation());
	ControlledTank->GetBarrelComponent()->Elevate(DeltaRotator.Pitch);
}

bool UTankAimingComponent::AimAt(FVector &TargetLocation)
{
	FVector StartLocation = ControlledTank->GetBarrelComponent()->GetProjectileLaunchLocation();
	FVector LaunchVelocity{FVector::ZeroVector};
	TArray<AActor *> ActorsToIgnore{ControlledTank};

	// Calc the LaunchVelocity
	bool bProjectileVelocitySuccess = UGameplayStatics::SuggestProjectileVelocity(
		this,
		LaunchVelocity,
		StartLocation,
		TargetLocation,
		ControlledTank->GetProjectileLaunchSpeed(),
		false,
		0,
		0,
		ESuggestProjVelocityTraceOption::DoNotTrace,
		FCollisionResponseParams::DefaultResponseParam,
		ActorsToIgnore,
		false);

	if (bProjectileVelocitySuccess)
	{
		AimDirection = FVector::ZeroVector;
		AimDirection = LaunchVelocity.GetSafeNormal();
		if (!AimDirection.IsZero())
		{
			MoveTurret();
			MoveBarrel();

			return true;
		}
	}

	return false;
}

void UTankAimingComponent::Fire()
{
	if (FiringState == EFiringState::Locked || FiringState == EFiringState::Aiming)
	{
		UE_LOG(LogTemp, Warning, TEXT("Fire!"));

		auto Barrel = ControlledTank->GetBarrelComponent();
		auto Projectile = GetWorld()->SpawnActor<ATankProjectile>(
			ControlledTank->GetTankProjectile(),
			Barrel->GetProjectileLaunchLocation(),
			Barrel->GetProjectileLaunchRotation());
		if (Projectile)
		{
			Projectile->Launch(ControlledTank->GetProjectileLaunchSpeed());

			UpdateAmmoCount();

			// Reset the timer
			LastFireTime = FPlatformTime::Seconds();
		}
	}
}