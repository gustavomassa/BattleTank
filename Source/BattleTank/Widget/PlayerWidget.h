// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerWidget.generated.h"

/**
 * 
 */
UCLASS()
class BATTLETANK_API UPlayerWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void Setup();

private:
	UPROPERTY(meta = (BindWidget))
	class UImage *Crosshair;
};
