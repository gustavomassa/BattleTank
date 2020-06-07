// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTurretComponent.h"
//#include "UObject/ConstructorHelpers.h"

UTankTurretComponent::UTankTurretComponent()
{
    //static ConstructorHelpers::FObjectFinder<UStaticMesh> TurretAsset(TEXT("/Game/Source/Models/Tank/SM_Tank_Turret"));
    //this->SetStaticMesh(TurretAsset.Object);
    this->SetRelativeLocation(FVector::ZeroVector);
}

void UTankTurretComponent::SetupPhysics()
{
    SetSimulatePhysics(false);
}

void UTankTurretComponent::Rotate(float TargetAngle, float MaxDegressPerSecond)
{
    /*     TargetAngle = (FMath::Abs(TargetAngle) < 180.0f) ? TargetAngle : -TargetAngle;
    float MaxLimitedAngle = (MaxDegressPerSecond * FMath::Sign(TargetAngle) * GetWorld()->DeltaTimeSeconds);
    float TargetRotation = (FMath::Abs(TargetAngle) > FMath::Abs(MaxLimitedAngle)) ? GetRelativeRotation().Yaw + MaxLimitedAngle : GetRelativeRotation().Yaw + TargetAngle; */

    // Limit the speed and invert the angle when passing 180 degrees
    TargetAngle = (FMath::Abs(TargetAngle) < 180.0f) ? TargetAngle : -TargetAngle;
    TargetAngle = FMath::Clamp<float>(TargetAngle, -1.0f, +1.0f);
    float RotationChange = (TargetAngle * MaxDegressPerSecond * GetWorld()->DeltaTimeSeconds);
    float CurrentRotation = GetRelativeRotation().Yaw;
    float TargetRotation = (CurrentRotation + RotationChange);

    //UE_LOG(LogTemp, Warning, TEXT("CurrentRotation: %f"), FMath::Abs(CurrentRotation));
    //UE_LOG(LogTemp, Warning, TEXT("TargetRotation: %f"), FMath::Abs(TargetRotation));

    if (FMath::Abs(TargetRotation) > 0.0f && FMath::Abs(TargetRotation) != FMath::Abs(CurrentRotation))
    {
        FRotator TargetRotator{0.0f, TargetRotation, 0.0f};
        SetRelativeRotation(TargetRotator);
    }
}
