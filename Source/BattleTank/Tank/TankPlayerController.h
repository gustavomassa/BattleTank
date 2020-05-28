// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Interface/TankControllerInterface.h"
#include "TankPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class BATTLETANK_API ATankPlayerController : public APlayerController, public ITankControllerInterface
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	ATank *GetControlledTank() const override;

private:
	FVector CrosshairHitLocation{FVector::ZeroVector};

	// Crosshair X is 50% alignment
	UPROPERTY(EditAnywhere)
	float CrosshairLocationX{0.5f};
	// Crosshair Y is 33.3% alignment
	UPROPERTY(EditAnywhere)
	float CrosshairLocationY{0.33333f};

	UPROPERTY(EditAnywhere)
	float CrosshairReachDistance{1000000.0f};

	FVector2D GetCrosshairScreenLocation() const;
	bool GetLookDirection(FVector2D &ScreenLocation, FVector &Out_LookDirection) const;
	bool GetSightRayHitLocation(FVector &Out_HitLocation) const;
	bool GetLookDirectionHitResult(FVector &LookDirection, FHitResult &Out_HitResult) const;
	void AimTowardsCrosshair();
	void FollowCrosshair();
};
