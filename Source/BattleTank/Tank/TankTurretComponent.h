// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "TankTurretComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BATTLETANK_API UTankTurretComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	UTankTurretComponent();
	void Rotate(float TargetAngle);

private:
	UPROPERTY(EditAnyWhere, Category = "Setup")
	float MaxDegressPerSecond{50.0f};
};
