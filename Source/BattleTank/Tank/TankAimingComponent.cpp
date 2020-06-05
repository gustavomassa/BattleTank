// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAimingComponent.h"
#include "Tank.h"
#include "TankTurretComponent.h"
#include "TankBarrelComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

// Called when the game starts
void UTankAimingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

// Called every frame
void UTankAimingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UTankAimingComponent::SetTankTurretReference(UTankTurretComponent *TankTurretComponentToSet)
{
	TankTurretComponent = TankTurretComponentToSet;
}

void UTankAimingComponent::SetTankBarrelReference(UTankBarrelComponent *TankBarrelComponentToSet)
{
	TankBarrelComponent = TankBarrelComponentToSet;
}

void UTankAimingComponent::MoveTurret(FVector &TargetLocation)
{
	if (!TankTurretComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("%s: Failed to get Tank Turret Reference!"), *GetOwner()->GetName());
		return;
	}

	FRotator DeltaRotator = (TargetLocation.Rotation() - TankTurretComponent->GetForwardVector().Rotation());
	TankTurretComponent->Rotate(DeltaRotator.Yaw);
}

void UTankAimingComponent::MoveBarrel(FVector &TargetLocation)
{
	if (!TankBarrelComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("%s: Failed to get Tank Barrel Reference!"), *GetOwner()->GetName());
		return;
	}
	FRotator DeltaRotator = (TargetLocation.Rotation() - TankBarrelComponent->GetForwardVector().Rotation());
	TankBarrelComponent->Elevate(DeltaRotator.Pitch);
}

bool UTankAimingComponent::AimAt(FVector &TargetLocation)
{
	if (!TankBarrelComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("%s: Failed to get Tank Barrel Reference!"), *GetOwner()->GetName());
		return false;
	}

	auto ControlledTank = Cast<ATank>(GetOwner());
	FVector StartLocation = TankBarrelComponent->GetProjectileLaunchLocation();
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
		FVector AimDirection = LaunchVelocity.GetSafeNormal();
		if (!AimDirection.IsZero())
		{
			MoveTurret(AimDirection);
			MoveBarrel(AimDirection);

			return true;
		}
	}

	return false;
}

void UTankAimingComponent::Aim(FVector TargetLocation)
{
	MoveTurret(TargetLocation);
	MoveBarrel(TargetLocation);
}
