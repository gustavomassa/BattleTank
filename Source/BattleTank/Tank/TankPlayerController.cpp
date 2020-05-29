// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"
#include "Tank.h"
#include "TankTurretComponent.h"
#include "TankBarrelComponent.h"
#include "GameFramework/PlayerController.h"
#include "Components/ActorComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"

void ATankPlayerController::BeginPlay()
{
    Super::BeginPlay();

    UE_LOG(LogTemp, Warning, TEXT("PlayerController Begin Play"));

    ControlledTank = GetControlledTank();
    if (!ensure(ControlledTank))
    {
        UE_LOG(LogTemp, Error, TEXT("%s: Failed to get Controlled Tank!"), *GetOwner()->GetName());
    }

    if (!ensure(ControlledTank) || !ensure(ControlledTank->InputComponent))
    {
        UE_LOG(LogTemp, Error, TEXT("%s: Failed to find Controlled Tank Input Component!"), *GetOwner()->GetName());
    }

    /*     if (!ensure(ControlledTank) || !ensure(ControlledTank->GetCameraComponent()))
    {
        UE_LOG(LogTemp, Error, TEXT("%s: Failed to find Controlled Tank Camera Component!"), *GetOwner()->GetName());
    } */

    // Register Input Binds
    ControlledTank->InputComponent->BindAxis("AimAzimuth", this, &ATankPlayerController::OnAxisAzimuth);
    ControlledTank->InputComponent->BindAxis("AimElevation", this, &ATankPlayerController::OnAxisElevation);
    ControlledTank->InputComponent->BindAction(FireBind, IE_Pressed, ControlledTank, &ATank::OnFire);

    UE_LOG(LogTemp, Warning, TEXT("PlayerController tank: %s!"), *ControlledTank->GetName());
}

void ATankPlayerController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    AimTowardsCrosshair();
    //FollowCrosshair();
}

ATank *ATankPlayerController::GetControlledTank() const
{
    // Tank is a especialization of the Pawn (Subtype - Runtime Polymorphism)
    return Cast<ATank>(GetPawn());
}

void ATankPlayerController::OnAxisAzimuth(float AxisValue)
{
    if (!ControlledTank || !ControlledTank->GetCameraComponent())
    {
        UE_LOG(LogTemp, Error, TEXT("%s: Failed to find Controlled Tank Camera Component!"), *GetOwner()->GetName());
        return;
    }
    float AxisChange = (AxisValue * AzimuthSensitivity * GetWorld()->DeltaTimeSeconds);
    FRotator TargetRotator = ControlledTank->GetCameraComponent()->GetRelativeRotation();
    TargetRotator.Yaw = (TargetRotator.Yaw + AxisChange);
    ControlledTank->GetCameraComponent()->SetRelativeRotation(TargetRotator);
}

void ATankPlayerController::OnAxisElevation(float AxisValue)
{
    if (!ControlledTank || !ControlledTank->GetCameraComponent())
    {
        UE_LOG(LogTemp, Error, TEXT("%s: Failed to find Controlled Tank Camera Component!"), *GetOwner()->GetName());
        return;
    }
    float AxisChange = (AxisValue * AzimuthSensitivity * GetWorld()->DeltaTimeSeconds);
    FRotator TargetRotator = ControlledTank->GetCameraComponent()->GetRelativeRotation();
    TargetRotator.Pitch = FMath::Clamp<float>((TargetRotator.Pitch + AxisChange), MinElevationAngle, MaxElevationAngle);
    ControlledTank->GetCameraComponent()->SetRelativeRotation(TargetRotator);
}

// Move the Tank Barrel aiming where the player crosshair intersects the world
void ATankPlayerController::AimTowardsCrosshair()
{
    if (!ControlledTank)
    {
        return;
    }

    CrosshairHitLocation = FVector::ZeroVector;
    if (GetSightRayHitLocation(CrosshairHitLocation))
    {
        ControlledTank->AimAt(CrosshairHitLocation);
    }
}

void ATankPlayerController::FollowCrosshair()
{
    if (!ControlledTank)
    {
        return;
    }

    FVector2D ScreenLocation = GetCrosshairScreenLocation();
    FVector LookDirection{FVector::ZeroVector};
    if (GetLookDirection(ScreenLocation, LookDirection))
    {
        ControlledTank->Aim(LookDirection);
    }
}

FVector2D ATankPlayerController::GetCrosshairScreenLocation() const
{
    int32 ViewPortSizeX, ViewPortSizeY;
    GetViewportSize(ViewPortSizeX, ViewPortSizeY);
    return FVector2D(ViewPortSizeX * CrosshairLocationX, ViewPortSizeY * CrosshairLocationY);
}

bool ATankPlayerController::GetLookDirection(FVector2D &ScreenLocation, FVector &Out_LookDirection) const
{
    // De-project the screen position of the crosshair to a world direction (Unit Vector based on Pythagorus)
    FVector LookLocation{FVector::ZeroVector};
    return DeprojectScreenPositionToWorld(ScreenLocation.X, ScreenLocation.Y, LookLocation, Out_LookDirection);
}

bool ATankPlayerController::GetSightRayHitLocation(FVector &Out_HitLocation) const
{
    auto ScreenLocation = GetCrosshairScreenLocation();
    //UE_LOG(LogTemp, Warning, TEXT("Crosshair Screen Location: %s"), *ScreenLocation.ToString());

    FVector LookDirection{FVector::ZeroVector};
    if (GetLookDirection(ScreenLocation, LookDirection))
    {
        //UE_LOG(LogTemp, Warning, TEXT("Look Direction: %s"), *LookDirection.ToString());
        FHitResult HitResult;
        if (GetLookDirectionHitResult(LookDirection, HitResult))
        {
            //UE_LOG(LogTemp, Warning, TEXT("Hit: %s, Location: %s"), *HitResult.GetComponent()->GetName(), *HitResult.Location.ToString());
            Out_HitLocation = HitResult.Location;
            return true;
        }
    }

    Out_HitLocation = FVector::ZeroVector;
    return false;
}

bool ATankPlayerController::GetLookDirectionHitResult(FVector &LookDirection, FHitResult &Out_HitResult) const
{
    FVector StartLocation = PlayerCameraManager->GetCameraLocation();
    FVector EndLocation = StartLocation + (LookDirection * CrosshairReachDistance);

    // Ignore the Controlled Tank, we don't want to hit ourself
    FCollisionQueryParams TraceParams = FCollisionQueryParams::DefaultQueryParam;
    TraceParams.AddIgnoredActor(ControlledTank->GetUniqueID());
    TraceParams.AddIgnoredActor(ControlledTank->GetTurretComponent()->GetUniqueID());
    TraceParams.AddIgnoredActor(ControlledTank->GetBarrelComponent()->GetUniqueID());

    return (GetWorld()->LineTraceSingleByChannel(
        Out_HitResult,
        StartLocation,
        EndLocation,
        ECollisionChannel::ECC_Visibility,
        TraceParams));
}