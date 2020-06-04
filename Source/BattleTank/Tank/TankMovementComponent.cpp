// Fill out your copyright notice in the Description page of Project Settings.

#include "TankMovementComponent.h"
#include "TankTrackComponent.h"

void UTankMovementComponent::IntendMoveForward(float Throw)
{
    //UE_LOG(LogTemp, Warning, TEXT("IntendMoveForward: %f"), Throw);

    //TODO: Limit speed due to dual control
    TankTrackLeftComponent->SetThrottle(Throw);
    TankTrackRightComponent->SetThrottle(Throw);
}

void UTankMovementComponent::IntendMoveLeft(float Throw)
{
    //UE_LOG(LogTemp, Warning, TEXT("IntendMoveLeft: %f"), Throw);

    TankTrackLeftComponent->SetThrottle(-Throw);
    TankTrackRightComponent->SetThrottle(Throw);
}

void UTankMovementComponent::IntendMoveRight(float Throw)
{
    //UE_LOG(LogTemp, Warning, TEXT("IntendMoveRight: %f"), Throw);

    TankTrackLeftComponent->SetThrottle(Throw);
    TankTrackRightComponent->SetThrottle(-Throw);
}

void UTankMovementComponent::IntendMoveBackward(float Throw)
{
    //UE_LOG(LogTemp, Warning, TEXT("IntendMoveBackward: %f"), Throw);

    TankTrackLeftComponent->SetThrottle(-Throw);
    TankTrackRightComponent->SetThrottle(-Throw);
}

void UTankMovementComponent::RequestDirectMove(const FVector &MoveVelocity, bool bForceMaxSpeed)
{
    // No need to call Super as we're replacing the functionality
    //Super::RequestDirectMove(MoveVelocity, bForceMaxSpeed);

    // FString TankName = GetOwner()->GetName();
    //UE_LOG(LogTemp, Warning, TEXT("%s: vectoring to %s"), *TankName, *MoveVelocity.ToString());

    FVector TankForwardIntention = GetOwner()->GetActorForwardVector().GetSafeNormal();
    FVector AIForwardIntention = MoveVelocity.GetSafeNormal();

    // Coss
    float FowardThrow = FVector::DotProduct(TankForwardIntention, AIForwardIntention);

    // Sen
    float RightThrow = FVector::CrossProduct(TankForwardIntention, AIForwardIntention).Z;

    UE_LOG(LogTemp, Warning, TEXT("FowardThrow: %f, RightThrow: %f"), FowardThrow, RightThrow);

    if (FowardThrow > 0.0f)
    {
        IntendMoveForward(FowardThrow);
    }
    else
    {
        IntendMoveBackward(FMath::Abs(FowardThrow));
    }

    if (RightThrow > 0.0f)
    {
        IntendMoveRight(RightThrow);
    }
    else
    {
        IntendMoveLeft(FMath::Abs(RightThrow));
    }
}

void UTankMovementComponent::SetTankTrackLeftReference(UTankTrackComponent *TrackLeftToSet)
{
    TankTrackLeftComponent = TrackLeftToSet;
}

void UTankMovementComponent::SetTankTrackRightReference(UTankTrackComponent *TrackRightToSet)
{
    TankTrackRightComponent = TrackRightToSet;
}
