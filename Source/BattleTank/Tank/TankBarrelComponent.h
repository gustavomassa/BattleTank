// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "TankBarrelComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BATTLETANK_API UTankBarrelComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	UTankBarrelComponent();
	void Elevate(float TargetAngle);
	FVector GetProjectileLaunchLocation();

private:
	UPROPERTY(EditAnyWhere, Category = "Setup")
	float MaxDegressPerSecond{10.0f};

	UPROPERTY(EditAnyWhere, Category = "Setup")
	float MaxElevationDegress{45.0f};

	UPROPERTY(EditAnyWhere, Category = "Setup")
	float MinElevationDegress{0.0f};
};
