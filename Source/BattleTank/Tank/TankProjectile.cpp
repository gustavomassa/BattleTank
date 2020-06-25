// Fill out your copyright notice in the Description page of Project Settings.

#include "TankProjectile.h"
#include "TankProjectileMovementComponent.h"
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
	TankProjectileMovementComponent = CreateDefaultSubobject<UTankProjectileMovementComponent>(FName("Movement Component"));
	TankProjectileMovementComponent->bAutoActivate = false;

	CollisionMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Collision Mesh"));
	CollisionMesh->SetNotifyRigidBodyCollision(true);
	//CollisionMesh->IgnoreComponentWhenMoving(CollisionMesh, true);
	//CollisionMesh->IgnoreActorWhenMoving(this, true);
	CollisionMesh->SetVisibility(false);
	SetRootComponent(CollisionMesh);

	LaunchBlast = CreateDefaultSubobject<UParticleSystemComponent>(FName("Launch Blast"));
	LaunchBlast->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	ImpactBlast = CreateDefaultSubobject<UParticleSystemComponent>(FName("Impact Blast"));
	ImpactBlast->bAutoActivate = false;
	ImpactBlast->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	ExplosionForce = CreateDefaultSubobject<URadialForceComponent>(FName("Explosion Force"));
	ExplosionForce->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
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

	SetRootComponent(ImpactBlast);
	CollisionMesh->DestroyComponent();

	// Ignore collisions with other tank projectiles
	if (OtherActor)
	{
		if (Cast<ATankProjectile>(OtherActor))
		{
			UE_LOG(LogTemp, Warning, TEXT("PROJECTILE HIT ANOTHER PROJECTILE %s"), *OtherActor->GetName());
			return;
		}
	}

	ExplosionForce->FireImpulse();
	LaunchBlast->Deactivate();
	ImpactBlast->Activate();

	UGameplayStatics::ApplyRadialDamage(
		this,
		ProjectileDamage,
		GetActorLocation(),
		ExplosionForce->Radius,
		UDamageType::StaticClass(),
		TArray<AActor *>() // Damage all actors on the radius
	);

	GetWorld()->GetTimerManager().SetTimer(DestroyTimer, this, &ATankProjectile::OnDestroyTimerExpire, DestroyDelay, false);
}

void ATankProjectile::OnDestroyTimerExpire()
{
	Destroy();
}
