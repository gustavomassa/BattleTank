// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "../Interface/TankControllerInterface.h"
#include "TankAIController.generated.h"

// Forward Declarations

/**
 * 
 */
UCLASS()
class BATTLETANK_API ATankAIController : public AAIController, public ITankControllerInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnyWhere, Category = "Setup")
	float AcceptanceRadius{5000.0f};

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetPawn(APawn *InPawn) override;

private:
	ATank *ControlledTank{nullptr};
	ATank *PlayerTank{nullptr};
	FVector PlayerTankLocation{FVector::ZeroVector};

	UFUNCTION()
	void OnTankDeath();
};
