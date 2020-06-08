// Copyright Gustavo Massaneiro

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MortarPawn.generated.h"

// Forward Declarations
class UTankAimingComponent;
class URotateComponent;
class UBarrelComponent;

UCLASS()
class BATTLETANK_API AMortarPawn : public APawn
{
	GENERATED_BODY()

public:
	AMortarPawn();
	virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;

	UTankAimingComponent *GetTankAimingComponent() const;
	URotateComponent *GetTurretComponent() const;
	UBarrelComponent *GetBarrelComponent() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UTankAimingComponent *TankAimingComponent{nullptr};
	URotateComponent *TurretComponent{nullptr};
	UBarrelComponent *BarrelComponent{nullptr};

	void FindTurretComponent();
	void FindBarrelComponent();
};
