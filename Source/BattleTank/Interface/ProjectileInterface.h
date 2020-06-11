// Gustavo Massaneiro

#pragma once

#include "CoreMinimal.h"
#include "ProjectileInterface.generated.h"

//Forward declarations

UINTERFACE(MinimalAPI, Blueprintable)
class UProjectileInterface : public UInterface
{
    GENERATED_BODY()
};

class IProjectileInterface
{
    GENERATED_BODY()

public:
    virtual void BeginPlay() = 0;                                       // Register OnHit
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) = 0; // Clear Timers

    virtual void Launch(float Speed) = 0;
    virtual void OnHit(UPrimitiveComponent *HitComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit) = 0;
    virtual void OnDestroyTimerExpire() = 0;
};
