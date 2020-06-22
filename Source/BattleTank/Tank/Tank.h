// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Tank.generated.h"

//Foward Declarations
class UCameraComponent;
class ATankProjectile;
class UWidgetComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTankDelegate);

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

	FTankDelegate OnDeath;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USpringArmComponent *SpringArm{nullptr};

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCameraComponent *ThirdPersonCamera{nullptr};

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCameraComponent *FirstPersonCamera{nullptr};

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

	UPROPERTY(EditAnywhere, Category = "Input Axis")
	FName ThrottleLeftBind{"LeftTrackThrottle"};
	UPROPERTY(EditAnywhere, Category = "Input Axis")
	FName ThrottleRightBind{"RightTrackThrottle"};
	UPROPERTY(EditAnywhere, Category = "Input Axis")
	FName MoveForwardBind{"MoveForward"};
	UPROPERTY(EditAnywhere, Category = "Input Axis")
	FName MoveBackwardBind{"MoveBackward"};

	UPROPERTY(EditAnywhere, Category = "Input Action")
	FName SwitchCameraBind{"SwitchCamera"};
	UPROPERTY(EditAnywhere, Category = "Input Action")
	FName FireBind{"Fire"};

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	uint16 StartingHealth{100};
	UPROPERTY(VisibleAnywhere, Category = "Health")
	uint16 CurrentHealth{0};
	UFUNCTION(BlueprintPure, Category = "Health")
	float GetHealthPercent() const;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UWidgetComponent *HealthBar{nullptr};

	void SwitchCamera();
	virtual float TakeDamage(float Damage, struct FDamageEvent const &DamageEvent, AController *EventInstigator, AActor *DamageCauser) override;
};
