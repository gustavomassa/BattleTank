// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TankAimingComponent.generated.h"

//Foward Declaration
class UTankTurretComponent;
class UTankBarrelComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BATTLETANK_API UTankAimingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UTankAimingComponent();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	void SetTankTurretReference(UTankTurretComponent *TankTurretComponentToSet);
	void SetTankBarrelReference(UTankBarrelComponent *TankBarrelComponentToSet);
	bool AimAt(FVector &TargetLocation, FVector &Out_AimDirection);
	void Aim(FVector TargetLocation);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UTankTurretComponent *TankTurretComponent{nullptr};
	UTankBarrelComponent *TankBarrelComponent{nullptr};

	void MoveTurret(FVector &TargetLocation);
	void MoveBarrel(FVector &TargetLocation);
};
