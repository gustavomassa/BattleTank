// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTrackComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Tank.h"
#include "TankMovementComponent.h"

UTankTrackComponent::UTankTrackComponent()
{
    PrimaryComponentTick.bCanEverTick = false;

    // Object finder must be done in the constructor to make sure cooking project works fine
    static ConstructorHelpers::FObjectFinder<UStaticMesh> TrackAsset(TEXT("/Game/Source/Models/Tank/SM_Tank_Track"));
    this->SetStaticMesh(TrackAsset.Object);
    this->SetRelativeLocation(FVector::ZeroVector);
    this->SetSimulatePhysics(false);
    this->SetNotifyRigidBodyCollision(true);
}

void UTankTrackComponent::BeginPlay()
{
    Super::BeginPlay();

    ControlledTank = Cast<ATank>(GetOwner());
    if (!ensure(ControlledTank))
    {
        UE_LOG(LogTemp, Error, TEXT("%s: Failed to get Controlled Tank!"), *GetName());
    }
    TankRoot = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
    if (!ensure(TankRoot))
    {
        UE_LOG(LogTemp, Error, TEXT("%s: Failed to get Tank Root!"), *GetName());
    }

    OnComponentHit.AddDynamic(this, &UTankTrackComponent::OnHit);
}

/* void UTankTrackComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
} */

void UTankTrackComponent::SetThrottle(float Throttle)
{
    if (Throttle == 0.0f)
    {
        return;
    }
    CurrentThrottle = FMath::Clamp<float>(CurrentThrottle + Throttle, -1.0f, 1.0f);
}

void UTankTrackComponent::DriveTrack()
{
    if (CurrentThrottle == 0.0f)
    {
        return;
    }

    auto MovementComponent = ControlledTank->GetTankMovementComponent();
    if (!MovementComponent)
    {
        UE_LOG(LogTemp, Error, TEXT("%s: Failed to get Tank Movement Component"), *GetOwner()->GetName());
        return;
    }

    float TankMass = MovementComponent->GetMass();
    float MassRelation = (TankMass >= 1000.0f) ? (TankMass / 1000.0f) : 1.0f;
    float CurrentForceAdjustment = (MovementComponent->GetForceAdjustment() / FMath::Abs(CurrentThrottle));
    float TrackMaxDrivingForce = (TankMass * MovementComponent->GetGravityAcceleration() * MassRelation * CurrentForceAdjustment);

    //UE_LOG(LogTemp, Warning, TEXT("CurrentThrottle: %f, CurrentForceAdjustment: %f"), CurrentThrottle, CurrentForceAdjustment);

    FVector ForceApplied = (GetForwardVector() * CurrentThrottle * TrackMaxDrivingForce);
    TankRoot->AddForceAtLocation(ForceApplied, GetComponentLocation());
}

void UTankTrackComponent::CorrectSlippageForce()
{
    // Force = Mass * Acceleration
    // Acceleration = Speed / Time
    // DrawDebugVectors

    // Calculate the slippage speed
    FVector RightVector = GetRightVector();
    FVector CurrentComponentVelocity = GetComponentVelocity();

    // (Coss, RightVector is 90 Degress to the right of the tank heading) CurrentComponentVelocity * cos(90 + FacingDegress) = SlippageSpeed
    float SlippageSpeed = FVector::DotProduct(RightVector, CurrentComponentVelocity);

    // Work-out the required acceleration on this frame to correct
    FVector CorrectionAcceleration = -SlippageSpeed / GetWorld()->GetDeltaSeconds() * RightVector;

    //UE_LOG(LogTemp, Warning, TEXT("RightVector: %s, CurrentComponentVelocity: %s"), *RightVector.ToString(), *CurrentComponentVelocity.ToString());
    //UE_LOG(LogTemp, Warning, TEXT("SlippageSpeed: %f, CorrectionAcceleration: %s"), SlippageSpeed, *CorrectionAcceleration.ToString());

    FVector CorrectionForce = (TankRoot->GetMass() * CorrectionAcceleration) / 2; // Two tracks
    TankRoot->AddForce(CorrectionForce);
}

void UTankTrackComponent::OnHit(UPrimitiveComponent *HitComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit)
{
    DriveTrack();
    CorrectSlippageForce();

    // Reset throttle
    CurrentThrottle = 0.0f;
}
