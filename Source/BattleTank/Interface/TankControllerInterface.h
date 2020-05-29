// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Tank/Tank.h"
#include "TankControllerInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UTankControllerInterface : public UInterface
{
	GENERATED_BODY()
};

class ITankControllerInterface
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() = 0;
	virtual void Tick(float DeltaSeconds) = 0;
	virtual ATank *GetControlledTank() const = 0;
};
