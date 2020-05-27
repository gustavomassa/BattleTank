// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"
#include "TankBarrelComponent.h"

// Sets default values
ATank::ATank()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// No need to protect pointers as added at construction
	TankAimingComponent = CreateDefaultSubobject<UTankAimingComponent>(FName("Aiming Component"));
}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ATank::SetTankBarrelReference(UTankBarrelComponent *TankBarrelToSet)
{
	TankBarrelComponent = TankBarrelToSet;
	TankAimingComponent->SetTankBarrelReference(TankBarrelToSet);
}

void ATank::AimAt(FVector &HitLocation)
{
	TankAimingComponent->AimAt(HitLocation, LaunchSpeed);
}

void ATank::MoveBarrel(FVector &TargetLocation)
{
	if (!TankBarrelComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("%s: Failed to get Tank Barrel Reference!"), *GetOwner()->GetName());
		return;
	}
	FRotator DeltaRotator = (TargetLocation.Rotation() - TankBarrelComponent->GetForwardVector().Rotation());
	UE_LOG(LogTemp, Warning, TEXT("Delta Rotator: %s"), *DeltaRotator.ToString());
	// TODO: Limit Barrel Rotation

	//TankBarrelComponent->
}