// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CreateSessionWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API UCreateSessionWidgetBase : public UUserWidget
{
	GENERATED_BODY()
public:
	//버튼
	//-----------------------------
	//매칭 시작버튼
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UButton * CreateButton;
	UFUNCTION()
	void OnCreate();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UButton * BackButton;
	UFUNCTION()
	void OnBack();
	//-----------------------------

	//유저 이름 입력
	//-----------------------------
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UEditableTextBox * NameTextBox;
	UFUNCTION()
	void OnChangedText(const FText & name);
	//-----------------------------

	class UGameInfoInstance * pGameInstance;

	virtual void NativeConstruct() override;
};
