// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Interface/ProjectileInterface.h"
#include "TankProjectile.generated.h"

// Forward Declarations
class UTankProjectileMovementComponent;
class UParticleSystemComponent;
class URadialForceComponent;

UCLASS()
class BATTLETANK_API ATankProjectile : public AActor, public IProjectileInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATankProjectile();
	void Launch(float Speed);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UTankProjectileMovementComponent *TankProjectileMovementComponent{nullptr};

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent *CollisionMesh{nullptr};

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UParticleSystemComponent *LaunchBlast{nullptr};
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UParticleSystemComponent *ImpactBlast{nullptr};

	UPROPERTY(VisibleAnywhere, Category = "Components")
	URadialForceComponent *ExplosionForce{nullptr};

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float ProjectileDamage{20.0f};
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float DestroyDelay{5.0f};
	FTimerHandle DestroyTimer;

	UFUNCTION()
	void OnHit(UPrimitiveComponent *HitComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit);
	UFUNCTION()
	void OnDestroyTimerExpire();
};
