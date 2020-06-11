// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Tank.generated.h"

//Foward Declarations
class UCameraComponent;
class ATankProjectile;
class UWidgetComponent;

UCLASS()
class BATTLETANK_API ATank : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATank();
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;

	FORCEINLINE class USpringArmComponent *GetCameraComponent() const { return SpringArm; }
	FORCEINLINE class UTankBodyComponent *GetBodyComponent() const { return Body; }
	FORCEINLINE class URotateComponent *GetTurretComponent() const { return Turret; }
	FORCEINLINE class UBarrelComponent *GetBarrelComponent() const { return Barrel; }
	FORCEINLINE class UTankTrackComponent *GetTankTrackLeftComponent() const { return LeftTrack; }
	FORCEINLINE class UTankTrackComponent *GetTankTrackRightComponent() const { return RightTrack; }
	FORCEINLINE class UTankMovementComponent *GetTankMovementComponent() const { return Movement; }
	FORCEINLINE class UAimingComponent *GetTankAimingComponent() const { return Aiming; }

private:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USpringArmComponent *SpringArm{nullptr};

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCameraComponent *Camera{nullptr};

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UTankBodyComponent *Body{nullptr};

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UTankTrackComponent *LeftTrack{nullptr};

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UTankTrackComponent *RightTrack{nullptr};

	UPROPERTY(VisibleAnywhere, Category = "Components")
	URotateComponent *Turret{nullptr};

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UBarrelComponent *Barrel{nullptr};

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UTankMovementComponent *Movement{nullptr};

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UAimingComponent *Aiming{nullptr};

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	uint16 StartingHealth{100};
	UPROPERTY(VisibleAnywhere, Category = "Health")
	uint16 CurrentHealth{StartingHealth};

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UWidgetComponent *HealthBar{nullptr};

	virtual float TakeDamage(float Damage, struct FDamageEvent const &DamageEvent, AController *EventInstigator, AActor *DamageCauser) override;
};
