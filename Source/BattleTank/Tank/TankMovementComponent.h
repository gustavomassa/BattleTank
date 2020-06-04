// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/NavMovementComponent.h"
#include "TankMovementComponent.generated.h"

// Forward Declarations
class UTankTrackComponent;

/**
 * 
 */
UCLASS()
class BATTLETANK_API UTankMovementComponent : public UNavMovementComponent
{
	GENERATED_BODY()

public:
	void SetTankTrackLeftReference(UTankTrackComponent *TrackLeftToSet);
	void SetTankTrackRightReference(UTankTrackComponent *TrackRightToSet);
	void IntendMoveForward(float Throw);
	void IntendMoveLeft(float Throw);
	void IntendMoveRight(float Throw);
	void IntendMoveBackward(float Throw);

protected:
	virtual void RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed) override;

private:
	UTankTrackComponent *TankTrackLeftComponent{nullptr};
	UTankTrackComponent *TankTrackRightComponent{nullptr};

	float CurrentLeftThrottle{0.0f};
	float CurrentRightThrottle{0.0f};
};
