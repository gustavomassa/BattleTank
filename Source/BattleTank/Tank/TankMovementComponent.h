// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/NavMovementComponent.h"
#include "TankMovementComponent.generated.h"

// Forward Declarations
class ATank;
/**
 * 
 */
UCLASS()
class BATTLETANK_API UTankMovementComponent : public UNavMovementComponent
{
	GENERATED_BODY()

public:
	UTankMovementComponent();
	void IntendMoveForward(float Throw);
	void IntendMoveLeft(float Throw);
	void IntendMoveRight(float Throw);
	void IntendMoveBackward(float Throw);

protected:
	virtual void BeginPlay() override;
	virtual void RequestDirectMove(const FVector &MoveVelocity, bool bForceMaxSpeed) override;

private:
	ATank *ControlledTank{nullptr};
};
