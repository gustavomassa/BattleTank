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

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

private:
	float CurrentForceAdjustment{1.0f};
	float TrackMaxDrivingForce{1.0f};

	void CorrectSlippageForce(float DeltaTime);
	UFUNCTION()
	void OnHit(UPrimitiveComponent *HitComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit);
};
