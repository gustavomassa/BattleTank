// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"
#include "TankBodyComponent.h"
#include "../RotateComponent.h"
#include "../BarrelComponent.h"
#include "../AimingComponent.h"
#include "TankTrackComponent.h"
#include "TankMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
ATank::ATank()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Register Components

	//TODO: TURRET, BARREL, TRACKS
	Body = CreateDefaultSubobject<UTankBodyComponent>(FName("Body Mesh"));
	SetRootComponent(Body);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(FName("SpringArm Component"));
	SpringArm->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f));
	SpringArm->SetRelativeRotation(FRotator(-20.0f, 0.0f, 0.0f));
	SpringArm->TargetArmLength = 1000.0f;
	SpringArm->ProbeSize = 12.0f;
	SpringArm->ProbeChannel = ECollisionChannel::ECC_Camera;
	SpringArm->bDoCollisionTest = true;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritRoll = false;
	SpringArm->AttachToComponent(Body, FAttachmentTransformRules::KeepRelativeTransform);

	/* 	if (SpringArm->HasAnySockets())
	{
		TArray<FComponentSocketDescription> SocketList;
		SpringArm->QuerySupportedSockets(SocketList);

		for (auto Socket : SocketList)
		{
			UE_LOG(LogTemp, Warning, TEXT("SpringArm Socket: %s"), *Socket.Name.ToString());
		}
	} */

	Camera = CreateDefaultSubobject<UCameraComponent>(FName("Camera Component"));
	Camera->AttachToComponent(SpringArm, FAttachmentTransformRules::KeepRelativeTransform, FName("SpringEndpoint"));

	LeftTrack = CreateDefaultSubobject<UTankTrackComponent>(FName("Left Track Mesh"));
	LeftTrack->SetNotifyRigidBodyCollision(true);
	LeftTrack->AttachToComponent(Body, FAttachmentTransformRules::KeepRelativeTransform, FName("Track Left"));

	RightTrack = CreateDefaultSubobject<UTankTrackComponent>(FName("Right Track Mesh"));
	RightTrack->SetNotifyRigidBodyCollision(true);
	RightTrack->AttachToComponent(Body, FAttachmentTransformRules::KeepRelativeTransform, FName("Track Right"));

	Turret = CreateDefaultSubobject<URotateComponent>(FName("Turret Mesh"));
	Turret->AttachToComponent(Body, FAttachmentTransformRules::KeepRelativeTransform, FName("Turret"));

	Barrel = CreateDefaultSubobject<UBarrelComponent>(FName("Barrel Mesh"));
	Barrel->AttachToComponent(Turret, FAttachmentTransformRules::KeepRelativeTransform, FName("Barrel"));

	Movement = CreateDefaultSubobject<UTankMovementComponent>(FName("Movement Component"));
	Aiming = CreateDefaultSubobject<UAimingComponent>(FName("Aiming Component"));
}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}