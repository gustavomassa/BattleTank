// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "TankTrackComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), hideCategories = (Physics, Events))
class BATTLETANK_API UTankTrackComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	UTankTrackComponent();
	void SetupPhysics();
	void SetThrottle(float Throttle);

private:
	UPROPERTY(EditAnywhere, Category = "Setup")
	float GravityAcceleration{9.81f};
	UPROPERTY(EditAnywhere, Category = "Setup")
	float ForceAdjustment{2.1f};
};
