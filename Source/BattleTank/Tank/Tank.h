// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Tank.generated.h"

//Foward Declarations
class USpringArmComponent;
class UTankBodyComponent;
class URotateComponent;
class UBarrelComponent;
class UAimingComponent;
class UTankTrackComponent;
class UTankMovementComponent;
class ATankProjectile;

UCLASS()
class BATTLETANK_API ATank : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATank();
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;

	USpringArmComponent *GetCameraComponent() const;
	const UTankMovementComponent *GetTankMovementComponent() const;
	UAimingComponent *GetTankAimingComponent() const;
	const UTankBodyComponent *GetBodyComponent() const;
	URotateComponent *GetTurretComponent() const;
	UBarrelComponent *GetBarrelComponent() const;
	UTankTrackComponent *GetTankTrackLeftComponent() const;
	UTankTrackComponent *GetTankTrackRightComponent() const;

	float GetDefaultMass() const;
	float GetMass() const;
	TSubclassOf<ATankProjectile> GetTankProjectile() const;
	float GetProjectileLaunchSpeed() const;
	float GetReloadTimeInSeconds() const;
	float GetGravityAcceleration() const;
	float GetForceAdjustment() const;

private:
	UTankMovementComponent *TankMovementComponent{nullptr};

	// Firing
	UPROPERTY(EditAnywhere, Category = "Firing")
	TSubclassOf<ATankProjectile> TankProjectile;
	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	float ProjectileLaunchSpeed{10000.0f};
	UPROPERTY(EditAnywhere, Category = "Firing")
	float ReloadTimeInSeconds{3.0f};

	// Movement
	UPROPERTY(EditAnywhere, Category = "Movement")
	float GravityAcceleration{9.81f};
	UPROPERTY(EditAnywhere, Category = "Movement")
	float ForceAdjustment{1.9f};
	UPROPERTY(EditAnywhere, Category = "Movement")
	float Mass{40000.0f};
};
