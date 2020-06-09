// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "RotateComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BATTLETANK_API URotateComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	URotateComponent();
	void Rotate(float TargetAngle);
	float GetMaxDegressPerSecond() const;

protected:
	UPROPERTY(EditAnyWhere, Category = "Setup")
	float MaxDegressPerSecond{50.0f};
};
