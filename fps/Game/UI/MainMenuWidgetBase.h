// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API UMainMenuWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	//버튼
	//--------------------------------------------

	//방 생성버튼
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UButton * CreateButton;
	UFUNCTION()
	void OnCreate();


	//방 입장버튼
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UButton * JoinButton;
	UFUNCTION()
	void OnJoin();

	//종료버튼
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UButton * ExitButton;
	UFUNCTION()
	void OnExit();

	//--------------------------------------------

	class UGameInfoInstance * pGameInstance;

	virtual void NativeConstruct() override;

};
