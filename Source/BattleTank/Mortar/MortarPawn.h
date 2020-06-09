// Copyright Gustavo Massaneiro

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MortarPawn.generated.h"

// Forward Declarations
class URotateComponent;
class UBarrelComponent;
class UAimingComponent;

UCLASS()
class BATTLETANK_API AMortarPawn : public APawn
{
	GENERATED_BODY()

public:
	AMortarPawn();
	virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;

	UAimingComponent *GetTankAimingComponent() const;
	URotateComponent *GetTurretComponent() const;
	UBarrelComponent *GetBarrelComponent() const;
};
