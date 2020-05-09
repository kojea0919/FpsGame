// Fill out your copyright notice in the Description page of Project Settings.


#include "KillLogWidgetBase.h"
#include "Components/TextBlock.h"

void UKillLogWidgetBase::SetKillPlayerName(FText PlayerName)
{
	if(KillPlayerName)
		KillPlayerName->SetText(PlayerName);
}

void UKillLogWidgetBase::SetDeathPlayerName(FText PlayerName)
{
	if (DeathPlayerName)
		DeathPlayerName->SetText(PlayerName);
}

void UKillLogWidgetBase::NativeConstruct()
{
	KillPlayerName = Cast<UTextBlock>(GetWidgetFromName(TEXT("KillPlayerName")));
	
	DeathPlayerName = Cast<UTextBlock>(GetWidgetFromName(TEXT("DeathPlayerName")));
}
