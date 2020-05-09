// Fill out your copyright notice in the Description page of Project Settings.


#include "GameEndWidgetBase.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "GameInstance/GameInfoInstance.h"

void UGameEndWidgetBase::NativeConstruct()
{
	GameResult = Cast<UTextBlock>(GetWidgetFromName(TEXT("GameResult")));

	GoToMainMenu = Cast<UButton>(GetWidgetFromName(TEXT("GoToMainMenu")));
	if (GoToMainMenu)
	{
		GoToMainMenu->OnClicked.AddDynamic(this, &UGameEndWidgetBase::OnGoToMainMenu);
	}
}

void UGameEndWidgetBase::SetGameResult(bool bIsWin)
{
	if (bIsWin)
	{
		GameResult->SetText(FText::FromString(FString("Win")));
	}
	else
	{
		GameResult->SetText(FText::FromString(FString("Lose")));
	}
}

void UGameEndWidgetBase::OnGoToMainMenu()
{
	UGameInfoInstance * pGi = GetGameInstance<UGameInfoInstance>();
	if (pGi)
	{
		pGi->DestroySession();
	}
}
