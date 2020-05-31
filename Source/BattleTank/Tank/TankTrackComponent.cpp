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

    //TODO: LIMIT INPUT
    //TODO: Apply acceleration over time to a maximum value
    // Force = mass * acceleration
    // Acceleration = m/s
    // Calculate friction about 0.2
    // Tank mass is about 40 000 kg

    //TODO: Set tank mass from C++

    auto Tank = Cast<ATank>(GetOwner());

    float GravityAcceleration = 9.81f;
    float TankMass = Tank->GetMass();
    float TrackMaxDrivingForce = (TankMass * GravityAcceleration * (TankMass / 1000) * 2);

    UE_LOG(LogTemp, Warning, TEXT("TrackMaxDrivingForce: %f"), TrackMaxDrivingForce);

    FVector ForceApplied = (GetForwardVector() * Throttle * TrackMaxDrivingForce);
    FVector ForceLocation = GetComponentLocation();

    auto TankRoot = Cast<UPrimitiveComponent>(Tank->GetRootComponent());
    TankRoot->AddForceAtLocation(ForceApplied, ForceLocation);
}
