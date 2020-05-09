// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KillLogWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API UKillLogWidgetBase : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UTextBlock * KillPlayerName;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UTextBlock * DeathPlayerName;


	void SetKillPlayerName(FText PlayerName);

	void SetDeathPlayerName(FText PlayerName);



	virtual void NativeConstruct() override;


};
