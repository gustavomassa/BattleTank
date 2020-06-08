// Copyright Gustavo Massaneiro

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MortarAIController.generated.h"

// Forward Declarations
class AMortarPawn;
class ATank;

/**
 * 
 */
UCLASS()
class BATTLETANK_API AMortarAIController : public AAIController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	AMortarPawn *GetControlledMortar() const;

private:
	AMortarPawn *ControlledMortar{nullptr};
	ATank *PlayerTank{nullptr};
	FVector PlayerTankLocation{FVector::ZeroVector};
};
