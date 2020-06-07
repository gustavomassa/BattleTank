// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Interface/TankControllerInterface.h"
#include "TankPlayerController.generated.h"

//Forward Declarations
class USpringArmComponent;
class UMenuWidget;
class UPlayerWidget;

/**
 * 
 */
UCLASS()
class BATTLETANK_API ATankPlayerController : public APlayerController, public ITankControllerInterface
{
	GENERATED_BODY()

public:
	ATankPlayerController();

	void SetMainMenuWidgetReference(UMenuWidget *MenuWidgetToSet);
	void SetPlayerWidgetReference(UPlayerWidget *PlayerWidgetToSet);

	const UMenuWidget *GetMenuWidget() const;
	UPlayerWidget *GetPlayerWidget() const;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	ATank *GetControlledTank() const override;

private:
	UMenuWidget *MenuWidget{nullptr};
	UPlayerWidget *PlayerWidget{nullptr};

	ATank *ControlledTank{nullptr};

	// Crosshair X is 50% alignment
	float CrosshairLocationX{0.5f};
	// Crosshair Y is 33.3% alignment
	float CrosshairLocationY{0.33333f};
	UPROPERTY(EditAnywhere, Category = "Crosshair")
	float CrosshairReachDistance{1000000.0f};
	FVector CrosshairHitLocation{FVector::ZeroVector};

	UPROPERTY(EditAnywhere, Category = "Input Axis")
	FName AzimuthBind{"AimAzimuth"};
	UPROPERTY(EditAnywhere, Category = "Input Axis")
	float AzimuthSensitivity{100.0f};
	UPROPERTY(EditAnywhere, Category = "Input Axis")
	FName ElevationBind{"AimElevation"};
	UPROPERTY(EditAnywhere, Category = "Input Axis")
	float MinElevationAngle{-20.0f};
	UPROPERTY(EditAnywhere, Category = "Input Axis")
	float MaxElevationAngle{5.0f};
	UPROPERTY(EditAnywhere, Category = "Input Axis")
	float ElevationSensitivity{100.0f};
	UPROPERTY(EditAnywhere, Category = "Input Axis")
	FName ThrottleLeftBind{"LeftTrackThrottle"};
	UPROPERTY(EditAnywhere, Category = "Input Axis")
	FName ThrottleRightBind{"RightTrackThrottle"};
	UPROPERTY(EditAnywhere, Category = "Input Axis")
	FName MoveForwardBind{"MoveForward"};
	UPROPERTY(EditAnywhere, Category = "Input Axis")
	FName MoveBackwardBind{"MoveBackward"};

	UPROPERTY(EditAnywhere, Category = "Input Action")
	FName FireBind{"Fire"};

	//TODO: Create a camera component and delegate
	UFUNCTION(BlueprintCallable, Category = "Input Axis")
	void OnAxisAzimuth(float AxisValue);
	UFUNCTION(BlueprintCallable, Category = "Input Axis")
	void OnAxisElevation(float AxisValue);

	void RegisterInputBind() const;
	FVector2D GetCrosshairScreenLocation() const;
	bool GetLookDirection(const FVector2D &ScreenLocation, FVector &Out_LookDirection) const;
	bool GetSightRayHitLocation(FVector &Out_HitLocation) const;
	bool GetLookDirectionHitResult(const FVector &LookDirection, FHitResult &Out_HitResult) const;
	bool AimTowardsCrosshair();
};
