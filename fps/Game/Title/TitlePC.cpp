// Fill out your copyright notice in the Description page of Project Settings.


#include "TitlePC.h"
#include "GameInstance/GameInfoInstance.h"

void ATitlePC::BeginPlay()
{
	UGameInfoInstance * pGameInstance = Cast<UGameInfoInstance>(GetGameInstance());
	
	if (!pGameInstance)
	{
		UE_LOG(LogClass, Warning, TEXT("ATitlePC::BeginPlay / pGameInstance"));
		return;
	}
	pGameInstance->ShowMainMenu();
}
