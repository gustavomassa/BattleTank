// Fill out your copyright notice in the Description page of Project Settings.

#include "TankBarrelComponent.h"
#include "UObject/ConstructorHelpers.h"

UTankBarrelComponent::UTankBarrelComponent()
{
    static ConstructorHelpers::FObjectFinder<UStaticMesh> BarrelAsset(TEXT("/Game/Source/Models/SM_Tank_Barrel"));
    this->SetStaticMesh(BarrelAsset.Object);
}

void UTankBarrelComponent::Elevate(float RelativeSpeed)
{
    UE_LOG(LogTemp, Warning, TEXT("TOCSON"));
}
