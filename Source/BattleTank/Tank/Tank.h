// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Tank.generated.h"

//Foward Declarations
class UTankTurretComponent;
class UTankBarrelComponent;
class UTankAimingComponent;

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
	void SetTankCameraReference(USceneComponent *CameraToSet);

	UFUNCTION(BlueprintCallable, Category = "Setup")
	void SetTankTurretReference(UTankTurretComponent *TurretToSet);

	UFUNCTION(BlueprintCallable, Category = "Setup")
	void SetTankBarrelReference(UTankBarrelComponent *BarrelToSet);

	UFUNCTION(BlueprintCallable, Category = "Input Action")
	void OnFire();

	USceneComponent *GetCameraComponent() const;
	UTankTurretComponent *GetTurretComponent() const;
	UTankBarrelComponent *GetBarrelComponent() const;

	void AimAt(FVector &HitLocation);
	void Aim(FVector &TargetLocation);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	USceneComponent *TankCameraComponent{nullptr};

	UTankTurretComponent *TankTurretComponent{nullptr};
	UTankBarrelComponent *TankBarrelComponent{nullptr};
	UTankAimingComponent *TankAimingComponent{nullptr};
};
