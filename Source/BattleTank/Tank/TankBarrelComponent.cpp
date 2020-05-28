// Fill out your copyright notice in the Description page of Project Settings.

#include "TankBarrelComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Math/UnrealMathUtility.h"

UTankBarrelComponent::UTankBarrelComponent()
{
    static ConstructorHelpers::FObjectFinder<UStaticMesh> BarrelAsset(TEXT("/Game/Source/Models/SM_Tank_Barrel"));
    this->SetStaticMesh(BarrelAsset.Object);
}

FVector UTankBarrelComponent::GetProjectileLaunchLocation()
{
    FVector StartLocation = this->GetSocketLocation(FName("Projectile"));
    // Make sure we get the Projectile socket location
    if (StartLocation == this->GetComponentLocation())
    {
        UE_LOG(LogTemp, Error, TEXT("%s: Failed to get Tank Barrel Projectile Location!"), *GetOwner()->GetName());
        return FVector::ZeroVector;
    }
    return StartLocation;
}

void UTankBarrelComponent::Elevate(float RelativeSpeed)
{
    //RelativeSpeed = FMath::Clamp<float>(RelativeSpeed, -1.0f, +1.0f);
    float ElevationChange = (RelativeSpeed * MaxDegressPerSecond * GetWorld()->DeltaTimeSeconds);
    float RawNewElevation = (RelativeRotation.Pitch + ElevationChange);
    float TargetElevation = FMath::Clamp<float>(RawNewElevation, MinElevationDegress, MaxElevationDegress);
    SetRelativeRotation(FRotator(TargetElevation, 0, 0));
}
