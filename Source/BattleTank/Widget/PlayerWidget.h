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
	//FORCEINLINE class UProgressBar *GetHealthBar() { return HealthBar; }

	void Setup();
	void UpdateCrosshairColor(const FLinearColor &LinearColor);
	void UpdateAmmoText(const FText &Text);
	void UpdateHealthBarPercent(float Percent);
	void BindHealthBarPercentage(UObject *InObject, const FName &InFunctionName);

private:
	UPROPERTY(meta = (BindWidget))
	class UImage *Crosshair;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock *AmmoText;
	UPROPERTY(meta = (BindWidget))
	class UProgressBar *HealthBar;
};
