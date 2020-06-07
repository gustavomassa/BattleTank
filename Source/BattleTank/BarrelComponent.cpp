// Fill out your copyright notice in the Description page of Project Settings.

#include "BarrelComponent.h"
//#include "UObject/ConstructorHelpers.h"

UBarrelComponent::UBarrelComponent()
{
    // Class finder must be done in the constructor to make sure cooking project works fine
    //static ConstructorHelpers::FObjectFinder<UStaticMesh> BarrelAsset(TEXT("/Game/Source/Models/Tank/SM_Tank_Barrel"));
    //this->SetStaticMesh(BarrelAsset.Object);
    this->SetRelativeLocation(FVector::ZeroVector);
}

FName UBarrelComponent::GetProjectileSocketName() const
{
    return ProjectileSocketName;
}

FVector UBarrelComponent::GetProjectileLaunchLocation() const
{
    FVector StartLocation = this->GetSocketLocation(ProjectileSocketName);
    if (StartLocation == this->GetComponentLocation())
    {
        UE_LOG(LogTemp, Error, TEXT("%s: Failed to get Tank Barrel Projectile Location!"), *GetOwner()->GetName());
        return FVector::ZeroVector;
    }
    return StartLocation;
}

FRotator UBarrelComponent::GetProjectileLaunchRotation() const
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

void UBarrelComponent::SetupPhysics()
{
    SetSimulatePhysics(false);
}

float UBarrelComponent::GetMaxDegressPerSecond() const
{
    return MaxDegressPerSecond;
}

float UBarrelComponent::GetMinElevationDegress() const
{
    return MinElevationDegress;
}

float UBarrelComponent::GetMaxElevationDegress() const
{
    return MaxElevationDegress;
}

void UBarrelComponent::Elevate(float TargetAngle)
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
