// Fill out your copyright notice in the Description page of Project Settings.

#include "TankBarrelComponent.h"
#include "UObject/ConstructorHelpers.h"
//#include "Math/UnrealMathUtility.h"

UTankBarrelComponent::UTankBarrelComponent()
{
    // Class finder must be done in the constructor to make sure cooking project works fine
    static ConstructorHelpers::FObjectFinder<UStaticMesh> BarrelAsset(TEXT("/Game/Source/Models/SM_Tank_Barrel"));
    //this->SetStaticMesh(BarrelAsset.Object);
    //this->SetWorldLocation(FVector::ZeroVector);

    //UStaticMesh *Barrel = BarrelAsset.Object;
    //RootComponent = Barrel;
    //this->AttachTo()
    //RootComponent = BarrelAsset.Object;

    this->SetStaticMesh(BarrelAsset.Object);
    this->SetRelativeLocation(FVector::ZeroVector);
}

FName UTankBarrelComponent::GetProjectileSocketName() const
{
    return ProjectileSocketName;
}

FVector UTankBarrelComponent::GetProjectileLaunchLocation() const
{
    FVector StartLocation = this->GetSocketLocation(ProjectileSocketName);
    if (StartLocation == this->GetComponentLocation())
    {
        UE_LOG(LogTemp, Error, TEXT("%s: Failed to get Tank Barrel Projectile Location!"), *GetOwner()->GetName());
        return FVector::ZeroVector;
    }
    return StartLocation;
}

FRotator UTankBarrelComponent::GetProjectileLaunchRotation() const
{
    /*     FRotator StartRotation = this->GetSocketRotation(ProjectileSocketName);
    if (StartRotation == this->GetComponentRotation())
    {
        UE_LOG(LogTemp, Error, TEXT("%s: Failed to get Tank Barrel Projectile Rotation!"), *GetOwner()->GetName());
        return FRotator::ZeroRotator;
    }
    return StartRotation; */
    return this->GetSocketRotation(ProjectileSocketName);
}

void UTankBarrelComponent::SetupPhysics()
{
    SetSimulatePhysics(false);
}

void UTankBarrelComponent::Elevate(float TargetAngle)
{
    /*     // Limit the speed and invert the angle when passing 180 degrees
    TargetAngle = (FMath::Abs(TargetAngle) < 180.0f) ? TargetAngle : -TargetAngle;
    TargetAngle = FMath::Clamp<float>(TargetAngle, -1.0f, +1.0f);
    float ElevationChange = (TargetAngle * MaxDegressPerSecond * GetWorld()->DeltaTimeSeconds);
    float RawNewElevation = (GetRelativeRotation().Pitch + ElevationChange); */

    TargetAngle = (FMath::Abs(TargetAngle) < 180.0f) ? TargetAngle : -TargetAngle;
    float MaxLimitedAngle = (MaxDegressPerSecond * FMath::Sign(TargetAngle) * GetWorld()->DeltaTimeSeconds);
    float CurrentElevation = GetRelativeRotation().Pitch;
    float RawNewElevation = (FMath::Abs(TargetAngle) > FMath::Abs(MaxLimitedAngle)) ? CurrentElevation + MaxLimitedAngle : CurrentElevation + TargetAngle;
    float TargetElevation = FMath::Clamp<float>(RawNewElevation, MinElevationDegress, MaxElevationDegress);

    //UE_LOG(LogTemp, Warning, TEXT("CurrentElevation: %f"), CurrentElevation);
    //UE_LOG(LogTemp, Warning, TEXT("TargetElevation: %f"), TargetElevation);

    if (TargetElevation > 0.0f && TargetElevation != CurrentElevation)
    {
        FRotator TargetRotator{TargetElevation, 0.0f, 0.0f};
        SetRelativeRotation(TargetRotator);
    }
}
