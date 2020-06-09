// Gustavo Massaneiro

#pragma once

#include "CoreMinimal.h"
#include "PlayerControllerInterface.generated.h"

//Forward declarations
class UPlayerWidget;

UINTERFACE(MinimalAPI, Blueprintable)
class UPlayerControllerInterface : public UInterface
{
    GENERATED_BODY()
};

class IPlayerControllerInterface
{
    GENERATED_BODY()

public:
    virtual UPlayerWidget *GetPlayerWidget() const = 0;
    //virtual UPlayerWidget *GetPlayerWidget() const PURE_VIRTUAL(GetPlayerWidget, return nullptr)
};
