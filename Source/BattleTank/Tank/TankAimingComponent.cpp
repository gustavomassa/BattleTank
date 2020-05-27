// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAimingComponent.h"
#include "Tank.h"
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

void UTankAimingComponent::SetTankBarrelReference(UTankBarrelComponent *TankBarrelComponentToSet)
{
	TankBarrelComponent = TankBarrelComponentToSet;
}

void UTankAimingComponent::AimAt(FVector TargetLocation, float LaunchSpeed)
{
	if (!TankBarrelComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("%s: Failed to get Tank Barrel Reference!"), *GetOwner()->GetName());
		return;
	}
	FVector StartLocation = TankBarrelComponent->GetSocketLocation(FName("Projectile"));
	// Make sure we get the Projectile socket location
	if (StartLocation == TankBarrelComponent->GetComponentLocation())
	{
		UE_LOG(LogTemp, Error, TEXT("%s: Failed to get Tank Barrel Projectile Location!"), *GetOwner()->GetName());
		return;
	}

	FVector LaunchVelocity{FVector::ZeroVector};
	TArray<AActor *> ActorsToIgnore{GetOwner()};

	// Calc the LaunchVelocity
	bool bProjectileVelocitySuccess = UGameplayStatics::SuggestProjectileVelocity(
		this,
		LaunchVelocity,
		StartLocation,
		TargetLocation,
		LaunchSpeed,
		false,
		0,
		0,
		ESuggestProjVelocityTraceOption::OnlyTraceWhileAscending,
		FCollisionResponseParams::DefaultResponseParam,
		ActorsToIgnore,
		true);

	if (bProjectileVelocitySuccess)
	{
		FVector AimDirection = LaunchVelocity.GetSafeNormal();
		if (!AimDirection.IsZero())
		{

			ATank *ControlledTank = Cast<ATank>(GetOwner());
			ControlledTank->MoveBarrel(AimDirection);
			UE_LOG(LogTemp, Warning, TEXT("%s: Firing at %s with speed %f"), *GetOwner()->GetName(), *TargetLocation.ToString(), LaunchSpeed);
		}
	}
}
