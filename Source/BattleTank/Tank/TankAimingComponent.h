// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TankAimingComponent.generated.h"

//Foward Declaration
class ATank;
class ATankPlayerController;

UENUM()
enum class EFiringState : uint8
{
	Reloading,
	Aiming,
	Locked
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BATTLETANK_API UTankAimingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UTankAimingComponent();
	const EFiringState &GetFiringState() const;
	bool AimAt(FVector &TargetLocation);
	void Fire();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

private:
	ATank *ControlledTank{nullptr};
	ATankPlayerController *TankPlayerController{nullptr};

	FVector AimDirection{FVector::ZeroVector};
	EFiringState FiringState{EFiringState::Reloading};
	EFiringState LastFiringState{EFiringState::Reloading};
	double LastFireTime{0};

	bool IsCrosshairLocked(float Tolerance);
	bool AimTowardsCrosshair();
	void MoveTurret();
	void MoveBarrel();
	const FLinearColor GetFiringStateCrosshairColor() const;
};
