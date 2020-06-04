// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTrackComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Tank.h"

UTankTrackComponent::UTankTrackComponent()
{
    // Object finder must be done in the constructor to make sure cooking project works fine
    static ConstructorHelpers::FObjectFinder<UStaticMesh> TrackAsset(TEXT("/Game/Source/Models/SM_Tank_Track"));
    this->SetStaticMesh(TrackAsset.Object);
    this->SetRelativeLocation(FVector::ZeroVector);
}

void UTankTrackComponent::SetupPhysics()
{
    SetSimulatePhysics(false);
}

void UTankTrackComponent::SetThrottle(float Throttle)
{
    //TODO: Apply acceleration over time to a maximum value
    //TODO: Limit speed due to dual control

    if (Throttle == 0.0f)
    {
        return;
    }

    Throttle = FMath::Clamp<float>(Throttle, -1.0f, 1.0f);
    float CurrentForceAdjustment = (ForceAdjustment / FMath::Abs(Throttle));
    float TankMass = Cast<ATank>(GetOwner())->GetMass();
    float MassRelation = (TankMass >= 1000) ? (TankMass / 1000) : 1;
    float TrackMaxDrivingForce = (TankMass * GravityAcceleration * MassRelation * CurrentForceAdjustment);
    float ThrottleChange = (Throttle * TrackMaxDrivingForce);

    FVector ForceApplied = (GetForwardVector() * ThrottleChange);
    ForceApplied.X = FMath::Clamp<float>(ForceApplied.X, -TrackMaxDrivingForce / CurrentForceAdjustment, TrackMaxDrivingForce / CurrentForceAdjustment);
    ForceApplied.Y = FMath::Clamp<float>(ForceApplied.Y, -TrackMaxDrivingForce / CurrentForceAdjustment, TrackMaxDrivingForce / CurrentForceAdjustment);
    ForceApplied.Z = FMath::Clamp<float>(ForceApplied.Z, -TrackMaxDrivingForce / CurrentForceAdjustment, TrackMaxDrivingForce / CurrentForceAdjustment);

    auto TankRoot = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
    TankRoot->AddForceAtLocation(ForceApplied, GetComponentLocation());
}
