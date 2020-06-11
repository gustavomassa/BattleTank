// Copyright Gustavo Massaneiro

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AimingComponent.generated.h"

//Forward declarations
class IPlayerControllerInterface;
class URotateComponent;
class UBarrelComponent;

UENUM()
enum class EFiringState : uint8
{
	Reloading,
	Aiming,
	Locked,
	OutOfAmmo
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BATTLETANK_API UAimingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAimingComponent();

protected:
	//UPROPERTY(EditAnywhere, Category = "Setup", meta = (EditCondition = "bHasTurret"))
	URotateComponent *TurretComponent{nullptr};
	//UPROPERTY(EditAnywhere, Category = "Setup")
	UBarrelComponent *BarrelComponent{nullptr};

	UPROPERTY(EditDefaultsOnly, Category = "Setup", meta = (MustImplement = "ProjectileInterface"))
	TSubclassOf<AActor> Projectile;
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float ProjectileLaunchSpeed{8000.0f};
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float ReloadTimeInSeconds{3.0f};
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	bool bHasTurret{false};

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	bool bUseAmmo{false};
	UPROPERTY(EditDefaultsOnly, Category = "Setup", meta = (EditCondition = "bUseAmmo"))
	uint8 InitialAmmo{10};
	UPROPERTY(EditDefaultsOnly, Category = "Setup", meta = (EditCondition = "bUseAmmo"))
	uint8 MaxAmmo{20};
	UPROPERTY(VisibleAnywhere, Category = "Setup", meta = (EditCondition = "bUseAmmo"))
	uint8 AmmoCount{InitialAmmo};

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	APawn *GetControlledPawn() const;
	IPlayerControllerInterface *GetPlayerController() const;
	const FLinearColor GetFiringStateCrosshairColor() const;

	URotateComponent *FindTurretComponent();
	UBarrelComponent *FindBarrelComponent();

	bool IsCrosshairLocked(float Tolerance);
	void MoveTurret();
	void MoveBarrel();
	void AddAmmo(uint8 Amount);
	void RemoveAmmo(uint8 Amount);
	void UpdateAmmoUI();

private:
	bool bIsPlayerPawn{false};
	FVector AimDirection{FVector::ZeroVector};
	EFiringState FiringState{EFiringState::Reloading};
	EFiringState LastFiringState{EFiringState::Reloading};
	double LastFireTime{0};

public:
	const EFiringState &GetFiringState() const;
	const uint8 &GetAmmoCount() const;

	bool AimAt(FVector &TargetLocation);
	void Fire();
};
