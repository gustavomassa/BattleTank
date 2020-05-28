// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Tank.generated.h"

//Foward Declarations
class UTankAimingComponent;
class UTankBarrelComponent;

UCLASS()
class BATTLETANK_API ATank : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATank();
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Setup")
	void SetTankBarrelReference(UTankBarrelComponent *TankBarrelToSet);

	void AimAt(FVector &HitLocation);
	//void MoveBarrel(FVector &TargetLocation);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UTankBarrelComponent *TankBarrelComponent{nullptr};
	UTankAimingComponent *TankAimingComponent{nullptr};

	UPROPERTY(EditAnyWhere, Category = "Firing")
	float LaunchSpeed{100000.0f}; //1000 m/s
};
