// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Tank.h"
#include "TankTurretComponent.h"
#include "TankBarrelComponent.h"
#include "TankTrackComponent.h"
#include "TankMovementComponent.h"
#include "TankAimingComponent.h"
#include "../Widget/MenuWidget.h"
#include "../Widget/PlayerWidget.h"

ATankPlayerController::ATankPlayerController()
{
}

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

    // Register Input Binds
    RegisterInputBind();

    UE_LOG(LogTemp, Warning, TEXT("PlayerController tank: %s!"), *ControlledTank->GetName());
}

void ATankPlayerController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    AimTowardsCrosshair();
}

void ATankPlayerController::SetMainMenuWidgetReference(UMenuWidget *MenuWidgetToSet)
{
    MenuWidget = MenuWidgetToSet;
}

void ATankPlayerController::SetPlayerWidgetReference(UPlayerWidget *PlayerWidgetToSet)
{
    PlayerWidget = PlayerWidgetToSet;
}

const UMenuWidget *ATankPlayerController::GetMenuWidget() const
{
    return MenuWidget;
}

UPlayerWidget *ATankPlayerController::GetPlayerWidget() const
{
    return PlayerWidget;
}

ATank *ATankPlayerController::GetControlledTank() const
{
    // Tank is a especialization of the Pawn (Subtype - Runtime Polymorphism)
    return Cast<ATank>(GetPawn());
}

void ATankPlayerController::RegisterInputBind() const
{
    if (!ControlledTank)
    {
        UE_LOG(LogTemp, Error, TEXT("%s: Failed to get Controlled Tank!"), *GetOwner()->GetName());
    }

    // Axis
    ControlledTank->InputComponent->BindAxis(AzimuthBind, this, &ATankPlayerController::OnAxisAzimuth);
    ControlledTank->InputComponent->BindAxis(ElevationBind, this, &ATankPlayerController::OnAxisElevation);
    ControlledTank->InputComponent->BindAxis(ThrottleLeftBind, ControlledTank->GetTankMovementComponent(), &UTankMovementComponent::IntendMoveLeft);
    ControlledTank->InputComponent->BindAxis(ThrottleRightBind, ControlledTank->GetTankMovementComponent(), &UTankMovementComponent::IntendMoveRight);
    ControlledTank->InputComponent->BindAxis(MoveForwardBind, ControlledTank->GetTankMovementComponent(), &UTankMovementComponent::IntendMoveForward);
    ControlledTank->InputComponent->BindAxis(MoveBackwardBind, ControlledTank->GetTankMovementComponent(), &UTankMovementComponent::IntendMoveBackward);

    // Actions
    ControlledTank->InputComponent->BindAction(FireBind, IE_Pressed, ControlledTank->GetTankAimingComponent(), &UTankAimingComponent::Fire);
}

// Yaw //TODO: Create a Camera Component and put the logic there
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

// Pitch
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

FVector2D ATankPlayerController::GetCrosshairScreenLocation() const
{
    int32 ViewPortSizeX, ViewPortSizeY;
    GetViewportSize(ViewPortSizeX, ViewPortSizeY);
    return FVector2D(ViewPortSizeX * CrosshairLocationX, ViewPortSizeY * CrosshairLocationY);
}

bool ATankPlayerController::GetLookDirection(const FVector2D &ScreenLocation, FVector &Out_LookDirection) const
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

bool ATankPlayerController::GetLookDirectionHitResult(const FVector &LookDirection, FHitResult &Out_HitResult) const
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

// Move the Tank Barrel aiming where the player crosshair intersects the world
bool ATankPlayerController::AimTowardsCrosshair()
{
    if (!ControlledTank)
    {
        return false;
    }

    CrosshairHitLocation = FVector::ZeroVector;
    if (GetSightRayHitLocation(CrosshairHitLocation) && !CrosshairHitLocation.IsZero())
    {
        return ControlledTank->GetTankAimingComponent()->AimAt(CrosshairHitLocation);
    }

    return false;
}