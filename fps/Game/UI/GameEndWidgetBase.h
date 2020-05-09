// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameEndWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API UGameEndWidgetBase : public UUserWidget
{
	GENERATED_BODY()
public:
	//게임 결과
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UTextBlock * GameResult;

	//메인 메뉴로 가는 버튼
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UButton * GoToMainMenu;

	virtual void NativeConstruct()override;

	void SetGameResult(bool bIsWin);

	UFUNCTION()
	void OnGoToMainMenu();
};
