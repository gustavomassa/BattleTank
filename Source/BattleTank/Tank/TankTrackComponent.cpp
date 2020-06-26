// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTrackComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Tank.h"
#include "TankBodyComponent.h"
#include "TankMovementComponent.h"
#include "TankSuspension.h"
#include "../SpawnPoint.h"

UTankTrackComponent::UTankTrackComponent()
{
    PrimaryComponentTick.bCanEverTick = false;

    // Object finder must be done in the constructor to make sure cooking project works fine
    static ConstructorHelpers::FObjectFinder<UStaticMesh> TrackAsset(TEXT("/Game/Source/Models/Tank/SM_Tank_Track"));
    this->SetStaticMesh(TrackAsset.Object);
    this->SetRelativeLocation(FVector::ZeroVector);
    this->SetSimulatePhysics(false);
    //this->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    //this->SetCollisionObjectType(ECollisionChannel::ECC_OverlapAll_Deprecated);
    this->SetNotifyRigidBodyCollision(false);
}

void UTankTrackComponent::BeginPlay()
{
    Super::BeginPlay();

    ControlledTank = Cast<ATank>(GetOwner());
    if (!ensure(ControlledTank))
    {
        UE_LOG(LogTemp, Error, TEXT("%s: Failed to get Controlled Tank!"), *GetName());
    }
    /*     TankRoot = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
    if (!ensure(TankRoot))
    {
        UE_LOG(LogTemp, Error, TEXT("%s: Failed to get Tank Root!"), *GetName());
    } */
}

/* void UTankTrackComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
} */

TArray<ATankSuspension *> UTankTrackComponent::GetWheels() const
{
    TArray<ATankSuspension *> Result;
    TArray<USceneComponent *> Children;
    GetChildrenComponents(true, Children);
    for (auto Child : Children)
    {
        auto SpawnPointChild = Cast<USpawnPoint>(Child);
        if (SpawnPointChild)
        {
            auto SpawnedActor = SpawnPointChild->GetSpawnedActor();
            if (SpawnedActor)
            {
                auto TankSuspensionChild = Cast<ATankSuspension>(SpawnedActor);
                if (TankSuspensionChild)
                {
                    Result.Add(TankSuspensionChild);
                }
            }
        }
    }

    return Result;
}

void UTankTrackComponent::SetThrottle(float Throttle)
{
    if (Throttle == 0.0f)
    {
        return;
    }

    Throttle = FMath::Clamp<float>(Throttle, -1.0f, 1.0f);
    DriveTrack(Throttle);
}

void UTankTrackComponent::DriveTrack(float Throttle)
{
    if (Throttle == 0.0f)
    {
        return;
    }

    auto TankBody = ControlledTank->GetBodyComponent();
    if (!TankBody)
    {
        UE_LOG(LogTemp, Error, TEXT("%s: Failed to get Tank Body Component"), *GetOwner()->GetName());
        return;
    }

    float TankMass = TankBody->GetMass();
    float MassRelation = (TankMass >= 1000.0f) ? (TankMass / 1000.0f) : 1.0f;
    float CurrentForceAdjustment = (ControlledTank->GetTankMovementComponent()->GetForceAdjustment() / FMath::Abs(Throttle));
    float TrackMaxDrivingForce = (TankMass * TankBody->GetGravityAcceleration() * MassRelation * CurrentForceAdjustment);

    //UE_LOG(LogTemp, Warning, TEXT("CurrentThrottle: %f, CurrentForceAdjustment: %f"), CurrentThrottle, CurrentForceAdjustment);

    //FVector ForceApplied = (GetForwardVector() * CurrentThrottle * TrackMaxDrivingForce);
    //TankRoot->AddForceAtLocation(ForceApplied, GetComponentLocation());

    float ForceApplied = (Throttle * TrackMaxDrivingForce);
    auto Wheels = GetWheels();
    float ForcePerWheel = ForceApplied / Wheels.Num();

    for (auto Wheel : Wheels)
    {
        Wheel->AddDrivingForce(ForcePerWheel);
    }
}

/* void UTankTrackComponent::CorrectSlippageForce()
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
} */