// Copyright Gustavo Massaneiro

#include "AimingComponent.h"
#include "RotateComponent.h"
#include "BarrelComponent.h"
#include "Tank/TankProjectile.h"
#include "Interface/PlayerControllerInterface.h"
#include "Interface/ProjectileInterface.h"
#include "Widget/PlayerWidget.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UAimingComponent::UAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UAimingComponent::BeginPlay()
{
	Super::BeginPlay();

	// Check if Pawn Controller is a Player Controller
	//bIsPlayerPawn = GetControlledPawn()->GetController()->Implements<IPlayerControllerInterface>();
	auto PlayerController = Cast<IPlayerControllerInterface>(GetControlledPawn()->GetController());
	if (PlayerController)
	{
		bIsPlayerPawn = true;
	}

	// Find Turret
	if (bHasTurret)
	{
		TurretComponent = FindTurretComponent();
		if (!TurretComponent)
		{
			UE_LOG(LogTemp, Error, TEXT("%s: Failed to find Turret Component"), *GetName());
		}
	}

	// Find Barrel
	BarrelComponent = FindBarrelComponent();
	if (!BarrelComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("%s: Failed to find Barrel Component"), *GetName());
	}

	// Init reload timer (Start Reloading)
	LastFireTime = FPlatformTime::Seconds();

	// Init Player UI
	UpdateAmmoUI();
}

// Called every frame
void UAimingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	LastFiringState = FiringState;

	if (bCountAmmo && AmmoCount <= 0)
	{
		FiringState = EFiringState::OutOfAmmo;
	}
	else if ((FPlatformTime::Seconds() - LastFireTime) < ReloadTimeInSeconds)
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
	if (bIsPlayerPawn && FiringState != LastFiringState)
	{
		GetPlayerController()->GetPlayerWidget()->UpdateCrosshairColor(GetFiringStateCrosshairColor());
	}
}

APawn *UAimingComponent::GetControlledPawn() const
{
	return Cast<APawn>(GetOwner());
}

IPlayerControllerInterface *UAimingComponent::GetPlayerController() const
{
	if (bIsPlayerPawn)
	{
		IPlayerControllerInterface *PlayerController = Cast<IPlayerControllerInterface>(GetControlledPawn()->GetController());
		if (!PlayerController)
		{
			UE_LOG(LogTemp, Error, TEXT("%s: Failed to Get Player Controller: Pawn Controller does not implement the IPlayerControllerInterface"), *GetName());
		}
		return PlayerController;
	}
	return nullptr;
}

const EFiringState &UAimingComponent::GetFiringState() const
{
	return FiringState;
}

const uint8 &UAimingComponent::GetAmmoCount() const
{
	return AmmoCount;
}

const FLinearColor UAimingComponent::GetFiringStateCrosshairColor() const
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

URotateComponent *UAimingComponent::FindTurretComponent()
{
	return GetOwner()->FindComponentByClass<URotateComponent>();
}

UBarrelComponent *UAimingComponent::FindBarrelComponent()
{
	return GetOwner()->FindComponentByClass<UBarrelComponent>();
}

bool UAimingComponent::IsCrosshairLocked(float Tolerance)
{
	if (!BarrelComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("%s: Failed to find Barrel Component"), *GetName());
	}

	FVector BarrelDirection = BarrelComponent->GetForwardVector();
	bool bValidateLockX = FMath::Abs(BarrelDirection.X - AimDirection.X) <= Tolerance;
	bool bValidateLockY = FMath::Abs(BarrelDirection.Y - AimDirection.Y) <= Tolerance;
	bool bValidateLockZ = (FMath::Abs(BarrelDirection.Z) > Tolerance) ? FMath::Abs(BarrelDirection.Z - AimDirection.Z) <= Tolerance : true;
	return (bValidateLockX && bValidateLockY && bValidateLockZ);
}

void UAimingComponent::MoveTurret()
{
	if (bHasTurret && !TurretComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("%s: Failed to find Turret Component"), *GetName());
	}

	FRotator DeltaRotator = (AimDirection.Rotation() - TurretComponent->GetForwardVector().Rotation());
	TurretComponent->Rotate(DeltaRotator.Yaw);
}

void UAimingComponent::MoveBarrel()
{
	if (!BarrelComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("%s: Failed to find Barrel Component"), *GetName());
	}

	FRotator DeltaRotator = (AimDirection.Rotation() - BarrelComponent->GetForwardVector().Rotation());
	BarrelComponent->Elevate(DeltaRotator.Pitch);
}

//TODO: Add a range limit based on the TargetLocation
bool UAimingComponent::AimAt(FVector &TargetLocation)
{
	if (bHasTurret && !TurretComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("%s: Failed to find Turret Component"), *GetName());
	}

	if (!BarrelComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("%s: Failed to find Barrel Component"), *GetName());
	}

	FVector StartLocation = BarrelComponent->GetProjectileLaunchLocation();
	if (StartLocation.IsZero())
	{
		return false;
	}

	FVector LaunchVelocity{FVector::ZeroVector};
	TArray<AActor *> ActorsToIgnore{GetControlledPawn()};

	// Calc the LaunchVelocity
	bool bProjectileVelocitySuccess = UGameplayStatics::SuggestProjectileVelocity(
		this,
		LaunchVelocity,
		StartLocation,
		TargetLocation,
		ProjectileLaunchSpeed,
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
			if (bHasTurret)
			{
				MoveTurret();
			}
			MoveBarrel();

			return true;
		}
	}
	return false;
}

void UAimingComponent::Fire()
{
	if (FiringState == EFiringState::Locked || FiringState == EFiringState::Aiming)
	{
		UE_LOG(LogTemp, Warning, TEXT("Fire!"));

		if (!BarrelComponent)
		{
			UE_LOG(LogTemp, Error, TEXT("%s: Failed to find Barrel Component"), *GetName());
		}

		auto SpawnedProjectile = GetWorld()->SpawnActor<IProjectileInterface>(
			Projectile,
			BarrelComponent->GetProjectileLaunchLocation(),
			BarrelComponent->GetProjectileLaunchRotation());

		if (SpawnedProjectile)
		{
			SpawnedProjectile->Launch(ProjectileLaunchSpeed);

			UpdateAmmoCount();
			UpdateAmmoUI();

			// Reset the timer
			LastFireTime = FPlatformTime::Seconds();
		}
	}
}

void UAimingComponent::UpdateAmmoCount()
{
	if (bCountAmmo && AmmoCount > 0)
	{
		--AmmoCount;
	}
}

void UAimingComponent::UpdateAmmoUI()
{
	if (bIsPlayerPawn)
	{
		GetPlayerController()->GetPlayerWidget()->UpdateAmmoText(FText::FromString(FString::FromInt(AmmoCount)));
	}
}