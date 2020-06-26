// Fill out your copyright notice in the Description page of Project Settings.

#include "TankProjectile.h"
#include "TankProjectileMovementComponent.h"
#include "Tank.h"
#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/DamageType.h"

// Sets default values
ATankProjectile::ATankProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Register Components
	Root = CreateDefaultSubobject<USceneComponent>(FName("Scene Root"));
	SetRootComponent(Root);

	CollisionMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Collision Mesh"));
	CollisionMesh->SetSimulatePhysics(false);
	CollisionMesh->SetNotifyRigidBodyCollision(true);
	CollisionMesh->SetVisibility(true);
	CollisionMesh->SetHiddenInGame(false);
	CollisionMesh->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);

	TankProjectileMovementComponent = CreateDefaultSubobject<UTankProjectileMovementComponent>(FName("Movement Component"));
	TankProjectileMovementComponent->SetUpdatedComponent(CollisionMesh);
	TankProjectileMovementComponent->bAutoActivate = false;
	TankProjectileMovementComponent->bRotationFollowsVelocity = true;

	LaunchBlast = CreateDefaultSubobject<UParticleSystemComponent>(FName("Launch Blast"));
	LaunchBlast->bAutoActivate = true;
	LaunchBlast->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);

	ImpactBlast = CreateDefaultSubobject<UParticleSystemComponent>(FName("Impact Blast"));
	ImpactBlast->bAutoActivate = false;
	ImpactBlast->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);

	ExplosionForce = CreateDefaultSubobject<URadialForceComponent>(FName("Explosion Force"));
	ExplosionForce->bAutoActivate = true;
	ExplosionForce->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void ATankProjectile::BeginPlay()
{
	Super::BeginPlay();

	//ExplosionForce->ImpulseStrength = (TankBody->GetMass() * TankBody->GetGravityAcceleration());
	CollisionMesh->OnComponentHit.AddDynamic(this, &ATankProjectile::OnHit);
}

void ATankProjectile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}

void ATankProjectile::Launch(float Speed)
{
	UE_LOG(LogTemp, Warning, TEXT("Projectile fired at %f"), Speed);
	TankProjectileMovementComponent->SetVelocityInLocalSpace(FVector::ForwardVector * Speed);
	TankProjectileMovementComponent->Activate();
}

void ATankProjectile::OnHit(UPrimitiveComponent *HitComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("PROJECTILE HIT"));

	//SetRootComponent(ImpactBlast);
	CollisionMesh->DestroyComponent();

	// Ignore collisions with other tank projectiles
	/* 	if (OtherActor)
	{
		if (Cast<ATankProjectile>(OtherActor))
		{
			UE_LOG(LogTemp, Warning, TEXT("PROJECTILE HIT ANOTHER PROJECTILE %s"), *OtherActor->GetName());
			return;
		}
	} */

	ExplosionForce->SetWorldLocation(Hit.Location);
	ExplosionForce->FireImpulse();

	LaunchBlast->Deactivate();

	ImpactBlast->SetWorldLocation(Hit.Location);
	ImpactBlast->Activate();

	auto DamageCauser = Cast<ATank>(GetOwner());

	UGameplayStatics::ApplyRadialDamage(
		this,
		ProjectileDamage,
		Hit.Location,
		ExplosionForce->Radius,
		UDamageType::StaticClass(),
		TArray<AActor *>{DamageCauser},
		DamageCauser);

	GetWorld()->GetTimerManager().SetTimer(DestroyTimer, this, &ATankProjectile::OnDestroyTimerExpire, DestroyDelay, false);
}

void ATankProjectile::OnDestroyTimerExpire()
{
	Destroy();
}
