// Fill out your copyright notice in the Description page of Project Settings.

#include "TankMovementComponent.h"
#include "Tank.h"
#include "TankTrackComponent.h"

UTankMovementComponent::UTankMovementComponent()
{
    bCanEverAffectNavigation = true;
}

void UTankMovementComponent::BeginPlay()
{
    Super::BeginPlay();

    ControlledTank = Cast<ATank>(GetOwner());
    if (!ensure(ControlledTank))
    {
        UE_LOG(LogTemp, Error, TEXT("%s: Failed to get Controlled Tank!"), *GetOwner()->GetName());
    }
}

void UTankMovementComponent::IntendMoveForward(float Throw)
{
    //UE_LOG(LogTemp, Warning, TEXT("IntendMoveForward: %f"), Throw);

    ControlledTank->GetTankTrackLeftComponent()->SetThrottle(Throw);
    ControlledTank->GetTankTrackRightComponent()->SetThrottle(Throw);
}

void UTankMovementComponent::IntendMoveLeft(float Throw)
{
    //UE_LOG(LogTemp, Warning, TEXT("IntendMoveLeft: %f"), Throw);

    ControlledTank->GetTankTrackLeftComponent()->SetThrottle(-Throw);
    ControlledTank->GetTankTrackRightComponent()->SetThrottle(Throw);
}

void UTankMovementComponent::IntendMoveRight(float Throw)
{
    //UE_LOG(LogTemp, Warning, TEXT("IntendMoveRight: %f"), Throw);

    ControlledTank->GetTankTrackLeftComponent()->SetThrottle(Throw);
    ControlledTank->GetTankTrackRightComponent()->SetThrottle(-Throw);
}

void UTankMovementComponent::IntendMoveBackward(float Throw)
{
    //UE_LOG(LogTemp, Warning, TEXT("IntendMoveBackward: %f"), Throw);

    ControlledTank->GetTankTrackLeftComponent()->SetThrottle(-Throw);
    ControlledTank->GetTankTrackRightComponent()->SetThrottle(-Throw);
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

    //UE_LOG(LogTemp, Warning, TEXT("FowardThrow: %f, RightThrow: %f"), FowardThrow, RightThrow);

    IntendMoveForward(FowardThrow);
    IntendMoveRight(RightThrow);

    /*     if (FowardThrow > 0.0f)
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
    } */
}