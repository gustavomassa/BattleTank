// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "../Interface/TankControllerInterface.h"
#include "TankAIController.generated.h"

/**
 * 
 */
UCLASS()
class BATTLETANK_API ATankAIController : public AAIController, public ITankControllerInterface
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	ATank *GetControlledTank() const override;

private:
	ATank *ControlledTank{nullptr};
	ATank *PlayerTank{nullptr};
	FVector PlayerTankLocation{FVector::ZeroVector};

	ATank *GetPlayerTank() const;
};
