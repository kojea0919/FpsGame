// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlayGS.h"
#include "UnrealNetwork.h"
#include "Game/GamePlay/GamePlayPC.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Game/UI/PlayerMainWidgetBase.h"

void AGamePlayGS::OnRep_RedTeamKillScore()
{
	AGamePlayPC * pPc = Cast<AGamePlayPC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (pPc && pPc->PlayerMainWidget)
	{
		pPc->PlayerMainWidget->UpdateRedTeamKillScore(iRedTeamKillScore);
	}
}

void AGamePlayGS::OnRep_BlueTeamKillScore()
{
	AGamePlayPC * pPc = Cast<AGamePlayPC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (pPc && pPc->PlayerMainWidget)
	{
		pPc->PlayerMainWidget->UpdateBlueTeamKillScore(iBlueTeamKillScore);
	}
}

void AGamePlayGS::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGamePlayGS, iRedTeamKillScore);
	DOREPLIFETIME(AGamePlayGS, iBlueTeamKillScore);
}
