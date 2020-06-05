// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerWidget.generated.h"

//Forward Declarations
enum class EFiringState : uint8;

/**
 * 
 */
UCLASS()
class BATTLETANK_API UPlayerWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void Setup();
	void UpdateFiringStateCrosshairColor(EFiringState &FiringState);

private:
	EFiringState CurrentCrossbowFiringState;

	UPROPERTY(meta = (BindWidget))
	class UImage *Crosshair;

	void Tocson();
};
