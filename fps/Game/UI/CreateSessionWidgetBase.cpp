// Fill out your copyright notice in the Description page of Project Settings.


#include "CreateSessionWidgetBase.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "GameInstance/GameInfoInstance.h"

void UCreateSessionWidgetBase::OnCreate()
{
	pGameInstance->StartOnlineGame(NameTextBox->GetText());
}

void UCreateSessionWidgetBase::OnBack()
{
	RemoveFromViewport();
	pGameInstance->ShowMainMenu();
}

void UCreateSessionWidgetBase::OnChangedText(const FText & name)
{
	if (name.IsEmpty())
	{
		CreateButton->SetIsEnabled(false);
	}
	else
	{
		CreateButton->SetIsEnabled(true);
	}
}

void UCreateSessionWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	CreateButton = Cast<UButton>(GetWidgetFromName(TEXT("CreateButton")));
	BackButton = Cast<UButton>(GetWidgetFromName(TEXT("BackButton")));
	NameTextBox = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("NameTextBox")));


	CreateButton->SetIsEnabled(false);

	if (!CreateButton->OnClicked.IsBound())
	{
		CreateButton->OnClicked.AddDynamic(this, &UCreateSessionWidgetBase::OnCreate);
	}


	if (!BackButton->OnClicked.IsBound())
	{
		BackButton->OnClicked.AddDynamic(this, &UCreateSessionWidgetBase::OnBack);
	}


	if (!NameTextBox->OnTextChanged.IsBound())
	{
		NameTextBox->OnTextChanged.AddDynamic(this, &UCreateSessionWidgetBase::OnChangedText);
	}


	pGameInstance = Cast<UGameInfoInstance>(GetGameInstance());
	if (!pGameInstance)
	{
		UE_LOG(LogClass, Warning, TEXT("UMainMenuWidgetBase::NativeConstruct / pGameInstance"));
		return;
	}
}
