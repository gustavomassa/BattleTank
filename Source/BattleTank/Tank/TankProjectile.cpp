// Fill out your copyright notice in the Description page of Project Settings.

#include "TankProjectile.h"
#include "TankProjectileMovementComponent.h"
#include "Engine/World.h"

// Sets default values
ATankProjectile::ATankProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Register Components
	TankProjectileMovementComponent = CreateDefaultSubobject<UTankProjectileMovementComponent>(FName("Projectile Movement Component"));
	TankProjectileMovementComponent->bAutoActivate;
}

// Called when the game starts or when spawned
void ATankProjectile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATankProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATankProjectile::Launch(float Speed)
{
	UE_LOG(LogTemp, Warning, TEXT("%f Projectile fires ate %f"), GetWorld()->GetTimeSeconds(), Speed);
	TankProjectileMovementComponent->SetVelocityInLocalSpace(FVector::ForwardVector * Speed);
	TankProjectileMovementComponent->Activate();
}
