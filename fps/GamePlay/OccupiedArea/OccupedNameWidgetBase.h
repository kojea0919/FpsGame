// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OccupedNameWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API UOccupedNameWidgetBase : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
	class UImage * OccupiedInitColor;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UImage * OccupiedRedColor;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UImage * OccupiedBlueColor;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UTextBlock * OccupiedName;

	virtual void NativeConstruct() override;

	void SetOccupiedName(FText Name);
	void SetOccupiedTeamColor(bool bIsRedTeam);
	void SetOcuupiedInitColor();
};
