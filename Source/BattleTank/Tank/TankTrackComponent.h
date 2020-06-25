// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "TankTrackComponent.generated.h"

// Forward declarations
class ATank;
class ATankSuspension;

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BATTLETANK_API UTankTrackComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	UTankTrackComponent();
	void SetThrottle(float Throttle);

protected:
	virtual void BeginPlay() override;
	//virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

private:
	ATank *ControlledTank{nullptr};
	UPrimitiveComponent *TankRoot{nullptr};
	TArray<ATankSuspension *> GetWheels() const;

	void DriveTrack(float Throttle);
	//void CorrectSlippageForce();
};
