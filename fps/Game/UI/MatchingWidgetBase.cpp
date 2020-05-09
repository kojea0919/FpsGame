// Fill out your copyright notice in the Description page of Project Settings.


#include "MatchingWidgetBase.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "GameInstance/GameInfoInstance.h"

void UMatchingWidgetBase::OnMatching()
{
	pGameInstance->FindOnlineGames(true, true, NameTextBox->GetText());
}

void UMatchingWidgetBase::OnBack()
{
	RemoveFromViewport();
	pGameInstance->ShowMainMenu();
}

void UMatchingWidgetBase::OnChangedText(const FText & name)
{
	if (name.IsEmpty())
	{
		MatchButton->SetIsEnabled(false);
	}
	else
	{
		MatchButton->SetIsEnabled(true);
	}
}

void UMatchingWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	MatchButton = Cast<UButton>(GetWidgetFromName(TEXT("MatchButton")));
	BackButton = Cast<UButton>(GetWidgetFromName(TEXT("BackButton")));
	NameTextBox = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("NameTextBox")));


	MatchButton->SetIsEnabled(false);

	if (!MatchButton->OnClicked.IsBound())
	{
		MatchButton->OnClicked.AddDynamic(this, &UMatchingWidgetBase::OnMatching);
	}


	if (!BackButton->OnClicked.IsBound())
	{
		BackButton->OnClicked.AddDynamic(this, &UMatchingWidgetBase::OnBack);
	}


	if (!NameTextBox->OnTextChanged.IsBound())
	{
		NameTextBox->OnTextChanged.AddDynamic(this, &UMatchingWidgetBase::OnChangedText);
	}


	pGameInstance = Cast<UGameInfoInstance>(GetGameInstance());
	if (!pGameInstance)
	{
		UE_LOG(LogClass, Warning, TEXT("UMainMenuWidgetBase::NativeConstruct / pGameInstance"));
		return;
	}
}
