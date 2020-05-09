// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidgetBase.h"
#include "Components/Button.h"
#include "GameInstance/GameInfoInstance.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

void UMainMenuWidgetBase::OnCreate()
{
	RemoveFromParent();
	pGameInstance->ShowCreateMenu();
}

void UMainMenuWidgetBase::OnJoin()
{
	RemoveFromParent();
	pGameInstance->ShowMatchingMenu();
}

void UMainMenuWidgetBase::OnExit()
{
	APlayerController * pPc = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (pPc)
	{
		UKismetSystemLibrary::QuitGame(GetWorld(), pPc, EQuitPreference::Quit, false);
	}
}

void UMainMenuWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	CreateButton = Cast<UButton>(GetWidgetFromName(TEXT("CreateButton")));
	JoinButton = Cast<UButton>(GetWidgetFromName(TEXT("JoinButton")));
	ExitButton = Cast<UButton>(GetWidgetFromName(TEXT("ExitButton")));

	if (!CreateButton->OnClicked.IsBound())
	{
		CreateButton->OnClicked.AddDynamic(this, &UMainMenuWidgetBase::OnCreate);
	}

	if (!JoinButton->OnClicked.IsBound())
	{
		JoinButton->OnClicked.AddDynamic(this, &UMainMenuWidgetBase::OnJoin);
	}

	if (!ExitButton->OnClicked.IsBound())
	{
		ExitButton->OnClicked.AddDynamic(this, &UMainMenuWidgetBase::OnExit);
	}

	pGameInstance = Cast<UGameInfoInstance>(GetGameInstance());
	if (!pGameInstance)
	{
		UE_LOG(LogClass, Warning, TEXT("UMatchingWidgetBase::NativeConstruct / !pGameInstance"));
		return;
	}
}
