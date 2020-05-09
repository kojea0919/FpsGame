// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MatchingWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API UMatchingWidgetBase : public UUserWidget
{
	GENERATED_BODY()
public:
	//��ư
	//-----------------------------
	//��Ī ���۹�ư
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UButton * MatchButton;
	UFUNCTION()
	void OnMatching();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UButton * BackButton;
	UFUNCTION()
	void OnBack();
	//-----------------------------

	//���� �̸� �Է�
	//-----------------------------
	UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
	class UEditableTextBox * NameTextBox;
	UFUNCTION()
	void OnChangedText(const FText & name);
	//-----------------------------

	class UGameInfoInstance * pGameInstance;

	virtual void NativeConstruct() override;
};
