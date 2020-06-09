// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Interface/ProjectileInterface.h"
#include "TankProjectile.generated.h"

// Forward Declarations
class UTankProjectileMovementComponent;

UCLASS()
class BATTLETANK_API ATankProjectile : public AActor, public IProjectileInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATankProjectile();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void Launch(float Speed);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UTankProjectileMovementComponent *TankProjectileMovementComponent{nullptr};

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent *CollisionMesh{nullptr};

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UParticleSystemComponent *LaunchBlast{nullptr};
};
