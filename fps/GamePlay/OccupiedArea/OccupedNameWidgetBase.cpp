// Fill out your copyright notice in the Description page of Project Settings.


#include "OccupedNameWidgetBase.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UOccupedNameWidgetBase::NativeConstruct()
{
	OccupiedInitColor = Cast<UImage>(GetWidgetFromName(TEXT("OccupiedInitColor")));
	OccupiedRedColor = Cast<UImage>(GetWidgetFromName(TEXT("OccupiedRedColor")));
	OccupiedBlueColor = Cast<UImage>(GetWidgetFromName(TEXT("OccupiedBlueColor")));

	OccupiedRedColor->SetVisibility(ESlateVisibility::Hidden);
	OccupiedBlueColor->SetVisibility(ESlateVisibility::Hidden);

	OccupiedName = Cast<UTextBlock>(GetWidgetFromName(TEXT("OccupiedName")));
}

void UOccupedNameWidgetBase::SetOccupiedName(FText Name)
{
	if (OccupiedName)
	{
		OccupiedName->SetText(Name);
	}
}

void UOccupedNameWidgetBase::SetOccupiedTeamColor(bool bIsRedTeam)
{
	if (!OccupiedInitColor || !OccupiedRedColor || !OccupiedBlueColor)
		return;


	if (bIsRedTeam)
	{
		OccupiedInitColor->SetVisibility(ESlateVisibility::Hidden);
		OccupiedBlueColor->SetVisibility(ESlateVisibility::Hidden);
		OccupiedRedColor->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		OccupiedInitColor->SetVisibility(ESlateVisibility::Hidden);
		OccupiedBlueColor->SetVisibility(ESlateVisibility::Visible);
		OccupiedRedColor->SetVisibility(ESlateVisibility::Hidden);
	}
	
}

void UOccupedNameWidgetBase::SetOcuupiedInitColor()
{
	if (!OccupiedInitColor || !OccupiedRedColor || !OccupiedBlueColor)
		return;

	OccupiedInitColor->SetVisibility(ESlateVisibility::Visible);
	OccupiedBlueColor->SetVisibility(ESlateVisibility::Hidden);
	OccupiedRedColor->SetVisibility(ESlateVisibility::Hidden);
}
