// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "TankBodyComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), hideCategories = (Physics, Events))
class BATTLETANK_API UTankBodyComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	UTankBodyComponent();

protected:
	virtual void BeginPlay() override;
};
