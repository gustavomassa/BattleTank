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

	UFUNCTION(BlueprintCallable, Category = "Setup")
	void SetTankCameraReference(USpringArmComponent *CameraToSet);

	UFUNCTION(BlueprintCallable, Category = "Setup")
	void SetTankBodyReference(UTankBodyComponent *BodyToSet);

	UFUNCTION(BlueprintCallable, Category = "Setup")
	void SetTankTurretReference(UTankTurretComponent *TurretToSet);

	UFUNCTION(BlueprintCallable, Category = "Setup")
	void SetTankBarrelReference(UTankBarrelComponent *BarrelToSet);

	UFUNCTION(BlueprintCallable, Category = "Setup")
	void SetTankTrackLeftReference(UTankTrackComponent *TrackLeftToSet);

	UFUNCTION(BlueprintCallable, Category = "Setup")
	void SetTankTrackRightReference(UTankTrackComponent *TrackRightToSet);

	UFUNCTION(BlueprintCallable, Category = "Input Action")
	void Fire();

	USpringArmComponent *GetCameraComponent() const;
	UTankBodyComponent *GetBodyComponent() const;
	UTankTurretComponent *GetTurretComponent() const;
	UTankBarrelComponent *GetBarrelComponent() const;
	UTankTrackComponent *GetTankTrackLeftComponent() const;
	UTankTrackComponent *GetTankTrackRightComponent() const;

	float GetMass() const;
	float GetProjectileLaunchSpeed() const;
	float GetReloadTimeInSeconds() const;

	void AimAt(FVector &HitLocation);
	void Aim(FVector &TargetLocation);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	USpringArmComponent *TankCameraComponent{nullptr};

	UTankBodyComponent *TankBodyComponent{nullptr};
	UTankTurretComponent *TankTurretComponent{nullptr};
	UTankBarrelComponent *TankBarrelComponent{nullptr};
	UTankTrackComponent *TankTrackLeftComponent{nullptr};
	UTankTrackComponent *TankTrackRightComponent{nullptr};

	UTankAimingComponent *TankAimingComponent{nullptr};

	UPROPERTY(EditAnywhere, Category = "Setup")
	float Mass{40000.0f};

	UPROPERTY(EditAnywhere, Category = "Setup")
	TSubclassOf<ATankProjectile> TankProjectile;

	UPROPERTY(EditAnywhere, Category = "Setup")
	float ReloadTimeInSeconds{3.0f};

	double LastFireTime{0};

	void FindTankTrackComponents();

	/* 	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float ProjectileLaunchSpeed{8000.0f}; */
};
