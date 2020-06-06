// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTrackComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Tank.h"

UTankTrackComponent::UTankTrackComponent()
{
    PrimaryComponentTick.bCanEverTick = false;

    // Object finder must be done in the constructor to make sure cooking project works fine
    static ConstructorHelpers::FObjectFinder<UStaticMesh> TrackAsset(TEXT("/Game/Source/Models/SM_Tank_Track"));
    this->SetStaticMesh(TrackAsset.Object);
    this->SetRelativeLocation(FVector::ZeroVector);
}

void UTankTrackComponent::BeginPlay()
{
    Super::BeginPlay();

    OnComponentHit.AddDynamic(this, &UTankTrackComponent::OnHit);
}

void UTankTrackComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    //CorrectSlippageForce(DeltaTime);
}

void UTankTrackComponent::SetupPhysics()
{
    SetSimulatePhysics(false);
    SetNotifyRigidBodyCollision(true);
}

void UTankTrackComponent::SetThrottle(float Throttle)
{
    //TODO: Apply acceleration over time to a maximum value
    //TODO: Limit speed due to dual control

    if (Throttle == 0.0f)
    {
        return;
    }

    auto ControlledTank = Cast<ATank>(GetOwner());
    Throttle = FMath::Clamp<float>(Throttle, -1.0f, 1.0f);
    float TankMass = ControlledTank->GetMass();
    float MassRelation = (TankMass >= 1000) ? (TankMass / 1000) : 1;

    CurrentForceAdjustment = (ControlledTank->GetForceAdjustment() / FMath::Abs(Throttle));
    TrackMaxDrivingForce = (TankMass * ControlledTank->GetGravityAcceleration() * MassRelation * CurrentForceAdjustment);

    float ThrottleChange = (Throttle * TrackMaxDrivingForce);

    FVector ForceApplied = (GetForwardVector() * ThrottleChange);
    ForceApplied.X = FMath::Clamp<float>(ForceApplied.X, -TrackMaxDrivingForce / CurrentForceAdjustment, TrackMaxDrivingForce / CurrentForceAdjustment);
    ForceApplied.Y = FMath::Clamp<float>(ForceApplied.Y, -TrackMaxDrivingForce / CurrentForceAdjustment, TrackMaxDrivingForce / CurrentForceAdjustment);
    ForceApplied.Z = FMath::Clamp<float>(ForceApplied.Z, -TrackMaxDrivingForce / CurrentForceAdjustment, TrackMaxDrivingForce / CurrentForceAdjustment);

    auto TankRoot = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
    TankRoot->AddForceAtLocation(ForceApplied, GetComponentLocation());
}

void UTankTrackComponent::CorrectSlippageForce(float DeltaTime)
{
    // Force = Mass * Acceleration
    // Acceleration = Speed / Time
    // DrawDebugVectors

    // Calculate the slippage speed
    FVector RightVector = GetRightVector();
    FVector CurrentComponentVelocity = GetComponentVelocity();
    float SlippageSpeed = FVector::DotProduct(RightVector, CurrentComponentVelocity);

    // Work-out the required acceleration on this frame to correct
    FVector CorrectionAcceleration = -SlippageSpeed / DeltaTime * RightVector;

    //UE_LOG(LogTemp, Warning, TEXT("RightVector: %s, CurrentComponentVelocity: %s"), *RightVector.ToString(), *CurrentComponentVelocity.ToString());
    //UE_LOG(LogTemp, Warning, TEXT("SlippageSpeed: %f, CorrectionAcceleration: %s"), SlippageSpeed, *CorrectionAcceleration.ToString());

    auto TankRoot = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
    FVector CorrectionForce = (TankRoot->GetMass() * CorrectionAcceleration) / 2; // Two tracks
    TankRoot->AddForce(CorrectionForce);
}

void UTankTrackComponent::OnHit(UPrimitiveComponent *HitComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit)
{
    UE_LOG(LogTemp, Warning, TEXT("I'm hit"));
}
