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
#include "Components/WidgetComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "../Widget/HealthBarWidget.h"
#include "Components/ProgressBar.h"

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

	HealthBar = CreateDefaultSubobject<UWidgetComponent>(FName("HealthBar Widget"));
	static ConstructorHelpers::FClassFinder<UUserWidget> MenuWidgetClassFinder(TEXT("/Game/Source/UI/UI_HealthBar"));
	HealthBar->SetWidgetClass(MenuWidgetClassFinder.Class);
	HealthBar->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

void ATank::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Axis
	PlayerInputComponent->BindAxis(ThrottleLeftBind, GetTankMovementComponent(), &UTankMovementComponent::IntendMoveLeft);
	PlayerInputComponent->BindAxis(ThrottleRightBind, GetTankMovementComponent(), &UTankMovementComponent::IntendMoveRight);
	PlayerInputComponent->BindAxis(MoveForwardBind, GetTankMovementComponent(), &UTankMovementComponent::IntendMoveForward);
	PlayerInputComponent->BindAxis(MoveBackwardBind, GetTankMovementComponent(), &UTankMovementComponent::IntendMoveBackward);

	// Actions
	PlayerInputComponent->BindAction(FireBind, IE_Pressed, GetTankAimingComponent(), &UAimingComponent::Fire);
}

void ATank::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = StartingHealth;

	HealthBar->InitWidget();
	auto HealBarUserWidget = Cast<UHealthBarWidget>(HealthBar->GetUserWidgetObject());
	if (!HealBarUserWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("%s: Failed to Cast to UHealthBarWidget"), *GetOwner()->GetName());
		return;
	}
	HealBarUserWidget->GetHealthBar()->PercentDelegate.BindUFunction(this, FName("GetHealthPercent"));
}

float ATank::GetHealthPercent() const
{
	//UE_LOG(LogTemp, Warning, TEXT("ATank GetHealthPercent"));
	return (float)CurrentHealth / (float)StartingHealth;
}

float ATank::TakeDamage(float Damage, struct FDamageEvent const &DamageEvent, AController *EventInstigator, AActor *DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	uint32 DamagePoints = FPlatformMath::RoundToInt(Damage);
	uint32 DamageToApply = FMath::Clamp<uint32>(DamagePoints, 0, CurrentHealth);
	CurrentHealth -= DamageToApply;
	if (CurrentHealth <= 0)
	{
		if (GetOwner())
		{
			UE_LOG(LogTemp, Warning, TEXT("%s: DIED"), *GetOwner()->GetName());
		}

		//TODO: Implement death variable
		OnDeath.Broadcast();
	}

	UE_LOG(LogTemp, Warning, TEXT("Damage: %f, DamageToApply: %i"), Damage, DamageToApply);

	return DamageToApply;
}