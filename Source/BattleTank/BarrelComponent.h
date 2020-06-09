// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "BarrelComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), hideCategories = (Physics))
class BATTLETANK_API UBarrelComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	UBarrelComponent();
	float GetMaxDegressPerSecond() const;
	float GetMinElevationDegress() const;
	float GetMaxElevationDegress() const;
	FName GetProjectileSocketName() const;
	FVector GetProjectileLaunchLocation() const;
	FRotator GetProjectileLaunchRotation() const;
	void Elevate(float TargetAngle);

protected:
	UPROPERTY(EditAnyWhere, Category = "Setup")
	float MaxDegressPerSecond{15.0f};
	UPROPERTY(EditAnyWhere, Category = "Setup")
	float MinElevationDegress{0.0f};
	UPROPERTY(EditAnyWhere, Category = "Setup")
	float MaxElevationDegress{50.0f};

private:
	FName ProjectileSocketName{FName("Projectile")};
};
