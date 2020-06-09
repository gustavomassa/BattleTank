// Fill out your copyright notice in the Description page of Project Settings.

#include "TankBodyComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Tank.h"

UTankBodyComponent::UTankBodyComponent()
{
    // Object finder must be done in the constructor to make sure cooking project works fine
    static ConstructorHelpers::FObjectFinder<UStaticMesh> BodyAsset(TEXT("/Game/Source/Models/Tank/SM_Tank_Body"));
    this->SetStaticMesh(BodyAsset.Object);
    this->SetRelativeLocation(FVector::ZeroVector);

    this->SetSimulatePhysics(true);
    this->SetEnableGravity(true);
    this->SetLinearDamping(0.1f);
    this->SetAngularDamping(0.2f);
}

void UTankBodyComponent::BeginPlay()
{
    Super::BeginPlay();

    this->SetMassOverrideInKg(NAME_None, Cast<ATank>(GetOwner())->GetDefaultMass(), true);
}