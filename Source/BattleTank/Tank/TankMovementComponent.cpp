// Fill out your copyright notice in the Description page of Project Settings.

#include "TankMovementComponent.h"
#include "TankTrackComponent.h"

void UTankMovementComponent::IntendMoveForward(float Throw)
{
    UE_LOG(LogTemp, Warning, TEXT("IntendMoveForward: %f"), Throw);

    //TODO: Limit speed due to dual control
    TankTrackLeftComponent->SetThrottle(Throw);
    TankTrackRightComponent->SetThrottle(Throw);
}

void UTankMovementComponent::IntendMoveLeft(float Throw)
{
    UE_LOG(LogTemp, Warning, TEXT("IntendMoveLeft: %f"), Throw);

    TankTrackLeftComponent->SetThrottle(Throw);
}

void UTankMovementComponent::IntendMoveRight(float Throw)
{
    UE_LOG(LogTemp, Warning, TEXT("IntendMoveRight: %f"), Throw);

    TankTrackRightComponent->SetThrottle(Throw);
}

void UTankMovementComponent::IntendMoveBackward(float Throw)
{
    UE_LOG(LogTemp, Warning, TEXT("IntendMoveBackward: %f"), Throw);

    TankTrackLeftComponent->SetThrottle(-Throw);
    TankTrackRightComponent->SetThrottle(-Throw);
}

void UTankMovementComponent::SetTankTrackLeftReference(UTankTrackComponent *TrackLeftToSet)
{
    TankTrackLeftComponent = TrackLeftToSet;
}

void UTankMovementComponent::SetTankTrackRightReference(UTankTrackComponent *TrackRightToSet)
{
    TankTrackRightComponent = TrackRightToSet;
}
