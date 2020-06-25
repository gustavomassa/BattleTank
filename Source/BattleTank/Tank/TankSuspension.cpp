// Copyright Gustavo Massaneiro

#include "TankSuspension.h"
#include "Components/SphereComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"

// Sets default values
ATankSuspension::ATankSuspension()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// This is very important for the OnHit
	PrimaryActorTick.TickGroup = TG_PostPhysics;

	FName AxleComponentname = FName("Axle");
	FName WheelComponentname = FName("Wheel");

	BodyAxleConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(FName("BodyAxleConstraint"));
	// Angular Limits
	BodyAxleConstraint->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 45.0f);
	BodyAxleConstraint->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 45.0f);
	BodyAxleConstraint->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 45.0f);
	// Linear Limits
	BodyAxleConstraint->SetLinearXLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);
	BodyAxleConstraint->SetLinearYLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);
	BodyAxleConstraint->SetLinearZLimit(ELinearConstraintMotion::LCM_Free, 0.0f);
	// Linear Motor
	BodyAxleConstraint->SetLinearPositionDrive(false, false, true);
	BodyAxleConstraint->SetLinearPositionTarget(FVector(0.0f));
	BodyAxleConstraint->SetLinearVelocityDrive(false, false, true);
	BodyAxleConstraint->SetLinearVelocityTarget(FVector(0.0f));
	BodyAxleConstraint->SetLinearDriveParams(1000.0f, 400.0f, 0.0f);

	BodyAxleConstraint->ComponentName2 = FConstrainComponentPropName{AxleComponentname};
	BodyAxleConstraint->SetVisibility(false);
	SetRootComponent(BodyAxleConstraint);

	Axle = CreateDefaultSubobject<USphereComponent>(AxleComponentname);
	Axle->SetSimulatePhysics(true);
	Axle->SetEnableGravity(true);
	// Collision
	Axle->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Axle->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	Axle->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

	Axle->SetMassOverrideInKg(NAME_None, 1000.0f, true);
	Axle->SetRelativeLocation(FVector(0.0f, 0.0f, -140.0f));
	Axle->SetVisibility(false);
	Axle->AttachToComponent(BodyAxleConstraint, FAttachmentTransformRules::KeepRelativeTransform);

	Wheel = CreateDefaultSubobject<USphereComponent>(WheelComponentname);
	Wheel->SetSimulatePhysics(true);
	Wheel->SetEnableGravity(true);
	Wheel->SetNotifyRigidBodyCollision(true);
	Wheel->SetCollisionProfileName(FName("PhysicsActor"));
	Wheel->SetMassOverrideInKg(NAME_None, 1000.0f, true);
	Wheel->SetVisibility(false);
	Wheel->AttachToComponent(Axle, FAttachmentTransformRules::KeepRelativeTransform);

	AxleWheelConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(FName("AxleWheelConstraint"));
	// Linear Limits
	AxleWheelConstraint->SetLinearXLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);
	AxleWheelConstraint->SetLinearYLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);
	AxleWheelConstraint->SetLinearZLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);
	// Angular Limits
	AxleWheelConstraint->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 45.0f);
	AxleWheelConstraint->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Free, 45.0f);
	AxleWheelConstraint->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 45.0f);

	AxleWheelConstraint->ComponentName1 = FConstrainComponentPropName{AxleComponentname};
	AxleWheelConstraint->ComponentName2 = FConstrainComponentPropName{WheelComponentname};
	AxleWheelConstraint->SetVisibility(false);
	AxleWheelConstraint->AttachToComponent(Axle, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void ATankSuspension::BeginPlay()
{
	Super::BeginPlay();

	auto ParentActor = GetAttachParentActor();
	if (ParentActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("ParentActor: %s"), *ParentActor->GetName());
		UPrimitiveComponent *BodyRoot = Cast<UPrimitiveComponent>(ParentActor->GetRootComponent());
		if (BodyRoot)
		{
			BodyAxleConstraint->SetConstrainedComponents(BodyRoot, NAME_None, Axle, NAME_None);
			AxleWheelConstraint->SetConstrainedComponents(Axle, NAME_None, Wheel, NAME_None);

			Wheel->OnComponentHit.AddDynamic(this, &ATankSuspension::OnHit);
		}
	}
}

// Called every frame
void ATankSuspension::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetWorld()->TickGroup == TG_PostPhysics)
	{
		// Physics is already completed and processes for this frame, reset for the next frame
		CurrentForce = 0.0f;
	}
}

void ATankSuspension::ApplyForce()
{
	if (CurrentForce != 0.0f)
	{
		Wheel->AddForce(Axle->GetForwardVector() * CurrentForce);
	}
}

void ATankSuspension::AddDrivingForce(float Force)
{
	if (Force != 0.0f)
	{
		CurrentForce += Force;
	}
}

void ATankSuspension::OnHit(UPrimitiveComponent *HitComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("%s: OnHit"), *GetName());

	ApplyForce();
}
