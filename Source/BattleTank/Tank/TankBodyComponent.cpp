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
}

void UTankBodyComponent::SetupPhysics()
{
    auto Tank = Cast<ATank>(GetOwner());
    SetSimulatePhysics(true);
    SetEnableGravity(true);
    SetMassOverrideInKg(NAME_None, Tank->GetDefaultMass(), true);
    SetLinearDamping(0.1f);
    SetAngularDamping(0.2f);
}
