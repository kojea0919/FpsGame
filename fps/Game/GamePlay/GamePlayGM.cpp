// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlayGM.h"
#include "Game/GamePlay/GamePlayPC.h"
#include "Game/GamePlay/GamePlayGS.h"
#include "Player/FpsPlayer.h"
#include "GameFramework/Character.h"
#include "GamePlay/PlayerSpawnPoint/PlayerSpawnPoint.h"
#include "GamePlay/OccupiedArea/OccupiedArea.h"
#include "Engine/Public/EngineUtils.h"
#include "Engine/World.h"
#include "TimerManager.h"


AGamePlayGM::AGamePlayGM()
{
	FpsPlayerSpawnLocation.Init(FVector(0.0f), iMaxPlayerNum);
}

void AGamePlayGM::PostLogin(APlayerController * NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (iCurrentPlayerNum > iMaxPlayerNum)
		return;

	AGamePlayPC * pPc = Cast<AGamePlayPC>(NewPlayer);
	if (nullptr == pPc)
	{
		UE_LOG(LogClass, Warning, TEXT("AGamePlayGM::PostLogin / pPc nullptr"));
		return;
	}

	FpsPlayerList.Add(pPc);

	////////////////////////////////////////
	FString name("Player");
	name.Append(FString::FromInt(iCurrentPlayerNum));
	pPc->PlayerName = FText::FromString(name);
	////////////////////////////////////////

}

void AGamePlayGM::UpdateKillScore(bool bIsRedTeam)
{
	AGamePlayGS * pGs = GetGameState<AGamePlayGS>();
	if (!pGs)
	{
		UE_LOG(LogClass, Warning, TEXT("AGamePlayGM::UpdateKillScore / !pGs"));
	}

	if (!bIsRedTeam)
	{
		++iRedTeamKillScore;

		pGs->iRedTeamKillScore = iRedTeamKillScore;
		pGs->OnRep_RedTeamKillScore();

		if (iRedTeamKillScore == iWinKillScore)
		{
			GameEnd(true);
			return;
		}
	}
	else
	{
		++iBlueTeamKillScore;
		pGs->iBlueTeamKillScore = iBlueTeamKillScore;
		pGs->OnRep_BlueTeamKillScore();

		if (iBlueTeamKillScore == iWinKillScore)
		{
			GameEnd(false);
			return;
		}
	}
}

EPlayerTeamState AGamePlayGM::IsRedTeam(int iPlayerNumberID)
{
	//�� ����
	EPlayerTeamState PlayerTeam;
	if (iPlayerNumberID <= 3)
		PlayerTeam = EPlayerTeamState::e_RedTeam;
	else
		PlayerTeam = EPlayerTeamState::e_BlueTeam;

	int iCnt = 0;

	//Red, Blue�� ���� Spawn ��ġ ����
	for (TActorIterator<APlayerSpawnPoint> iter(GetWorld()); iter; ++iter)
	{
		APlayerSpawnPoint * CurSpawnPoint = *iter;
		
		//�ٸ� Player�� Set�� SpawnPoint�� pass
		if (CurSpawnPoint->bIsSetPlayer)
			continue;

		//�ٸ� ���� SpawnPoint�� pass
		if (CurSpawnPoint->bIsRedTeamSpawnPoint &&
			PlayerTeam == EPlayerTeamState::e_BlueTeam)
			continue;
		else if (!CurSpawnPoint->bIsRedTeamSpawnPoint &&
			PlayerTeam == EPlayerTeamState::e_RedTeam)
			continue;

		else
		{
			CurSpawnPoint->bIsSetPlayer = true;
			FpsPlayerSpawnLocation[iPlayerNumberID - 1] = CurSpawnPoint->GetActorLocation();
			break;
		}
	}

	return PlayerTeam;
}

void AGamePlayGM::IncreaseOccupiedSocre(bool bIsRedTeam)
{
	if (bIsRedTeam)
	{
		++iRedTeamOccupiedScore;
		if (iRedTeamOccupiedScore == iWinOccupiedScore)
		{
			GameEnd(true);
		}
	}
	else
	{
		++iBlueTeamOccupiedScore;
		if (iBlueTeamOccupiedScore == iWinOccupiedScore)
		{
			GameEnd(false);
		}
	}
}

void AGamePlayGM::DecreaseOccupiedSocre(bool bIsRedTeam)
{
	if (bIsRedTeam)
	{
		--iRedTeamOccupiedScore;
	}
	else
	{
		--iBlueTeamOccupiedScore;
	}
}

void AGamePlayGM::AddReadyPlayerNum()
{
	++iReadyPlayerNum;

	if (iReadyPlayerNum == iMaxPlayerNum)
	{
		for (int iCnt = 0; iCnt < iMaxPlayerNum; ++iCnt)
		{
			//Client ���� ���� Progress Bar Gauge Up
			FpsPlayerList[iCnt]->StoC_GameStartTimerOn(iSpawnTime);
		}

		//�������� SpawnTime��ŭ ����� �÷��̾� ���� ��ġ�� �̵�
		GetWorldTimerManager().SetTimer(SpawnWaitHandler,
			this,
			&AGamePlayGM::SetPlayerStartLocation,
			iSpawnTime,
			false);

		TeamSetComplete();
	}
}

void AGamePlayGM::PlayerDead(int iKilPlayerNumberID, int iDeathPlayerNumberID)
{
	//���� �÷��̾� Kill Log
	int iLen = FpsPlayerList.Num();
	for (int iCnt = 0; iCnt < iLen; ++iCnt)
	{
		FpsPlayerList[iCnt]->StoC_ShowKillLog(FpsPlayerList[iKilPlayerNumberID-1]->PlayerName,
			FpsPlayerList[iDeathPlayerNumberID-1]->PlayerName);
	}

	//Client Progress Bar Update
	FpsPlayerList[iDeathPlayerNumberID - 1]->StoC_RespawnStartTimerOn(iRespwanTime);

	FTimerDelegate RespawnDelegate = FTimerDelegate::CreateUObject(this,
		&AGamePlayGM::SetPlayerRespawn, iDeathPlayerNumberID);

	//�������� ��� �� �ٽ� ����
	GetWorldTimerManager().SetTimer(RespawnWaitHandler[iDeathPlayerNumberID -1],
		RespawnDelegate,
		iRespwanTime,
		false);
}

void AGamePlayGM::SetPlayerStartLocation()
{
	for (int iCnt = 0; iCnt < iMaxPlayerNum; ++iCnt)
	{
		FpsPlayerList[iCnt]->GetCharacter()->SetActorLocation(FpsPlayerSpawnLocation[iCnt]);
	}
}

void AGamePlayGM::SetPlayerRespawn(int iPlayerNumberID)
{
	AFpsPlayer * pCurPlayer = Cast<AFpsPlayer>(FpsPlayerList[iPlayerNumberID - 1]->GetCharacter());
	if (pCurPlayer)
	{
		pCurPlayer->SetActorLocation(FpsPlayerSpawnLocation[iPlayerNumberID - 1]);

		//�÷��̾� �����·� �����ϴ� �۾�
		pCurPlayer->SetRespawnPlayerState();
	}

	GetWorldTimerManager().ClearTimer(RespawnWaitHandler[iPlayerNumberID - 1]);
}

void AGamePlayGM::TeamSetComplete()
{
	for (int iCnt = 0; iCnt < iMaxPlayerNum; ++iCnt)
	{
		AFpsPlayer * pCurPlayer = Cast<AFpsPlayer>(FpsPlayerList[iCnt]->GetCharacter());
		if (!pCurPlayer)
		{
			UE_LOG(LogClass, Warning, TEXT("AGamePlayGM::TeamSetComplete / !pCurPlayer"));
			return;
		}

		if(iCnt < 3)
			pCurPlayer->StoA_UpdateTeamWidget(true);
		else
			pCurPlayer->StoA_UpdateTeamWidget(false);
	}
}

void AGamePlayGM::GameEnd(bool bIsRedTeamWin)
{
	int iLen = FpsPlayerList.Num();
	for (int iCnt = 0; iCnt < iLen; ++iCnt)
	{
		//�̱� ���� ���
		FpsPlayerList[iCnt]->StoC_GameEnd(bIsRedTeamWin);
	}

	for (TActorIterator<AOccupiedArea> iter(GetWorld()); iter; ++iter)
	{
		AOccupiedArea * pArea = *iter;
		pArea->OverlapedPlayer.Empty();
		pArea->SetOccupiedAreaBoxNoCollision();
	}
}

