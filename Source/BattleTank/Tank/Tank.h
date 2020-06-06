// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Tank.generated.h"

//Foward Declarations
class USpringArmComponent;
class UTankBodyComponent;
class UTankTurretComponent;
class UTankBarrelComponent;
class UTankTrackComponent;
class UTankMovementComponent;
class UTankAimingComponent;
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

	void SetTankCameraReference(USpringArmComponent *CameraToSet);
	void SetTankBodyReference(UTankBodyComponent *BodyToSet);
	void SetTankTurretReference(UTankTurretComponent *TurretToSet);
	void SetTankBarrelReference(UTankBarrelComponent *BarrelToSet);
	void SetTankTrackLeftReference(UTankTrackComponent *TrackLeftToSet);
	void SetTankTrackRightReference(UTankTrackComponent *TrackRightToSet);

	USpringArmComponent *GetCameraComponent() const;
	const UTankMovementComponent *GetTankMovementComponent() const;
	const UTankBodyComponent *GetBodyComponent() const;
	const UTankTurretComponent *GetTurretComponent() const;
	const UTankBarrelComponent *GetBarrelComponent() const;
	const UTankTrackComponent *GetTankTrackLeftComponent() const;
	const UTankTrackComponent *GetTankTrackRightComponent() const;

	float GetDefaultMass() const;
	float GetMass() const;
	float GetProjectileLaunchSpeed() const;
	float GetReloadTimeInSeconds() const;
	float GetGravityAcceleration() const;
	float GetForceAdjustment() const;
	float GetTurretMaxDegressPerSecond() const;
	float GetBarrelMaxDegressPerSecond() const;
	float GetBarrelMinElevationDegress() const;
	float GetBarrelMaxElevationDegress() const;

	bool AimAt(FVector &HitLocation, FVector &Out_AimDirection);
	void Aim(FVector &TargetLocation);
	void Fire();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UTankMovementComponent *TankMovementComponent{nullptr};
	UTankAimingComponent *TankAimingComponent{nullptr};

private:
	USpringArmComponent *TankCameraComponent{nullptr};

	UTankBodyComponent *TankBodyComponent{nullptr};
	UTankTurretComponent *TankTurretComponent{nullptr};
	UTankBarrelComponent *TankBarrelComponent{nullptr};
	UTankTrackComponent *TankTrackLeftComponent{nullptr};
	UTankTrackComponent *TankTrackRightComponent{nullptr};

	// Projectile
	UPROPERTY(EditAnywhere, Category = "Setup")
	TSubclassOf<ATankProjectile> TankProjectile;
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float ProjectileLaunchSpeed{10000.0f};
	UPROPERTY(EditAnywhere, Category = "Setup")
	float ReloadTimeInSeconds{3.0f};

	// Movement
	UPROPERTY(EditAnywhere, Category = "Setup")
	float GravityAcceleration{9.81f};
	UPROPERTY(EditAnywhere, Category = "Setup")
	float ForceAdjustment{2.2f};
	UPROPERTY(EditAnywhere, Category = "Setup")
	float Mass{40000.0f};

	// Turret
	UPROPERTY(EditAnyWhere, Category = "Setup")
	float TurretMaxDegressPerSecond{50.0f};

	// Barrel
	UPROPERTY(EditAnyWhere, Category = "Setup")
	float BarrelMaxDegressPerSecond{15.0f};
	UPROPERTY(EditAnyWhere, Category = "Setup")
	float BarrelMinElevationDegress{0.0f};
	UPROPERTY(EditAnyWhere, Category = "Setup")
	float BarrelMaxElevationDegress{50.0f};

	void FindCameraComponent();
	void FindBodyComponent();
	void FindTurretComponent();
	void FindBarrelComponent();
	void FindTankTrackComponents();
};
