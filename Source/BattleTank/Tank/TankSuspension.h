// Copyright Gustavo Massaneiro

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TankSuspension.generated.h"

// Forward declarations
class USphereComponent;
class UPhysicsConstraintComponent;

UCLASS()
class BATTLETANK_API ATankSuspension : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATankSuspension();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void AddDrivingForce(float Force);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent *Axle{nullptr};
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent *Wheel{nullptr};
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPhysicsConstraintComponent *BodyAxleConstraint{nullptr};
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPhysicsConstraintComponent *AxleWheelConstraint{nullptr};

	float CurrentForce{0.0f};

	void ApplyForce();
	UFUNCTION()
	void OnHit(UPrimitiveComponent *HitComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit);
};
