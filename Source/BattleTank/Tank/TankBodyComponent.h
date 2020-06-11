// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "TankBodyComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BATTLETANK_API UTankBodyComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	UTankBodyComponent();
	FORCEINLINE float GetGravityAcceleration() const { return GravityAcceleration; }
	FORCEINLINE float GetDefaultMass() const { return Mass; }

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Setup")
	float GravityAcceleration{9.81f};
	UPROPERTY(EditAnywhere, Category = "Setup")
	float Mass{40000.0f};
};
