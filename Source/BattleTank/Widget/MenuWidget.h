// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class BATTLETANK_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void Setup();

private:
	UPROPERTY(meta = (BindWidget))
	class UButton *StartGameButton;
	UPROPERTY(meta = (BindWidgetOptional))
	class UButton *QuitGameButton;
	UPROPERTY(meta = (BindWidgetOptional))
	class UButton *ConfigGameButton;

	UFUNCTION()
	void OnStartGameButtonClicked();
};
