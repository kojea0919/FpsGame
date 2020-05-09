// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlayPC.h"
#include "Game/UI/PlayerMainWidgetBase.h"
#include "Game/UI/GameEndWidgetBase.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Game/GamePlay/GamePlayGM.h"
#include "Game/GamePlay/GamePlayGS.h"
#include "Player/FpsPlayer.h"
#include "Engine/World.h"
#include "UnrealNetwork.h"
#include "GameInstance/GameInfoInstance.h"


void AGamePlayPC::BeginPlay()
{
	if (IsLocalController())
	{

		if (PlayerMainWidgetClass)
		{
			PlayerMainWidget = CreateWidget<UPlayerMainWidgetBase>(this, PlayerMainWidgetClass);

			if (PlayerMainWidget)
			{
				PlayerMainWidget->SetHPBarRate(1.0f);
				PlayerMainWidget->AddToViewport();

				CtoS_SetTeam();
			}
		}

		if (GameEndWidgetClass)
		{
			GameEndWidget = CreateWidget<UGameEndWidgetBase>(this, GameEndWidgetClass);
		}

		UGameInfoInstance * pGi = GetGameInstance<UGameInfoInstance>();
		if (!pGi)
		{
			UE_LOG(LogClass, Warning, TEXT("AGamePlayPC::BeginPlay / !pGi"));
			return;
		}
		CtoS_SetPlayerID(pGi->LocalPlayerID);
	}
}

void AGamePlayPC::SetHPBar(float Rate)
{
	if (PlayerMainWidget)
	{
		PlayerMainWidget->SetHPBarRate(Rate);
	}
}

void AGamePlayPC::SetOnlyUI(bool bIsOnlyUI)
{
	if (bIsOnlyUI)
	{
		SetInputMode(FInputModeUIOnly());
	}
	else
	{
		SetInputMode(FInputModeGameOnly());
	}
}

void AGamePlayPC::UpdateRemainBulletUI(int iCurBulletCnt, int iAllBulletCnt)
{
	if (PlayerMainWidget)
	{
		PlayerMainWidget->UpdateRemainBulletCnt(iCurBulletCnt, iAllBulletCnt);
	}
}

void AGamePlayPC::ShowSingleUI()
{
	PlayerMainWidget->SetSingleUIVisibility(true);
	GetWorldTimerManager().SetTimer(OpacityHandler,
		this,
		&AGamePlayPC::SingleModeUIOnEffect,
		0.1f,
		false);
}

void AGamePlayPC::ShowRepeatUI()
{
	PlayerMainWidget->SetRepeatUIVisibility(true);
	GetWorldTimerManager().SetTimer(OpacityHandler,
		this,
		&AGamePlayPC::RepeatModeUIOnEffect,
		0.1f,
		false);
}

void AGamePlayPC::OnRep_PlayerTeam()
{
	if (!IsLocalController())
	{
		return;
	}

	bool bIsRedTeam;
	if (PlayerTeam == EPlayerTeamState::e_RedTeam)
	{
		bIsRedTeam = true;
	}
	else
	{
		bIsRedTeam = false;
	}

	PlayerMainWidget->SetTeam(bIsRedTeam);

	CtoS_ReadyComplete();
}

bool AGamePlayPC::IsRedTeam()
{
	if (PlayerTeam == EPlayerTeamState::e_RedTeam)
	{
		UE_LOG(LogClass, Warning, TEXT("red"));
		return true;
	}
	else if(PlayerTeam == EPlayerTeamState::e_BlueTeam)
	{
		UE_LOG(LogClass, Warning, TEXT("blue"));
		return false;
	}
	else
	{
		UE_LOG(LogClass, Warning, TEXT("NoSet"));
		return false;
	}
}


void AGamePlayPC::StoC_GameEnd_Implementation(bool bIsRedTeamWin)
{
	PlayerMainWidget->RemoveFromParent();
	GameEndWidget->AddToViewport();

	SetInputMode(FInputModeUIOnly());
	bShowMouseCursor = true;

	if (bIsRedTeamWin && IsRedTeam())
	{
		GameEndWidget->SetGameResult(true);
	}
	else if(!bIsRedTeamWin && !IsRedTeam())
	{
		GameEndWidget->SetGameResult(true);
	}
	else
	{
		GameEndWidget->SetGameResult(false);
	}
}

void AGamePlayPC::CtoS_SetTeam_Implementation()
{
	AGamePlayGM * pGm = Cast<AGamePlayGM>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!pGm)
	{
		UE_LOG(LogClass, Warning, TEXT("AGamePlayPC::SetTeam / !pGm"));
		return;
	}

	//Number ID 설정
	//----------------------------------------
	iPlayerNumberID = pGm->GetPlayerNumberID();
	//----------------------------------------

	//Team 설정
	//----------------------------------------
	PlayerTeam = pGm->IsRedTeam(iPlayerNumberID);

	OnRep_PlayerTeam();
	//----------------------------------------

}

void AGamePlayPC::CtoS_SetPlayerID_Implementation(const FText & PlayerID)
{
	PlayerName = PlayerID;
}

void AGamePlayPC::CtoS_ReadyComplete_Implementation()
{
	AGamePlayGM * pGm = Cast<AGamePlayGM>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!pGm)
	{
		UE_LOG(LogClass, Warning, TEXT("AGamePlayPC::ReadyComplete / !pGm"));
		return;
	}

	pGm->AddReadyPlayerNum();
}

void AGamePlayPC::StoC_GameStartTimerOn_Implementation(int iSpawnTime)
{
	if (PlayerMainWidget)
	{
		PlayerMainWidget->SetCountDownUIVisibility(true);
	}

	iCurrentRemainTime = iSpawnTime;
	GetWorldTimerManager().SetTimer(
		GameStartTimeHandler,
		this,
		&AGamePlayPC::SetGameStartRemainTime,
		1.0f,
		false);
}

void AGamePlayPC::StoC_RespawnStartTimerOn_Implementation(int iSpawnTime)
{
	//Spawn관련 UI 보이게 설정
	//----------------------------------------------
	if (PlayerMainWidget)
	{
		PlayerMainWidget->SetSpawnUIVisibility(true);
	}
	//----------------------------------------------

	ReSpawnWaitTime = iSpawnTime;


	//iSpawnTime = Respawn대기 시간
	//IncreaseRate = 호출당 progressbar 증가율
	//iSpawnTime : 1.0f = ? : IncreaseRate
	// ? = IncreaseRate * iSpawnTime
	GetWorldTimerManager().SetTimer(
		RespawnTimeHandler,
		this,
		&AGamePlayPC::SetRespawnRemainTime,
		IncreaseRate * iSpawnTime,
		false);
}

void AGamePlayPC::StoC_ShowKillLog_Implementation(const FText & KillPlayerID, const FText & DeathPlayerID)
{
	if (PlayerMainWidget)
	{
		//현재 킬로그 창이 꽉차있는 경우
		if (iMaxKillLogCnt <= iKillLogCnt)
		{
			//가장 오래된 Kill Log 삭제
			ShowEndKillLog(iFirstHandlerIdx);
		}

		//킬로그 생성
		PlayerMainWidget->CreateNewKillLog(KillPlayerID, DeathPlayerID);
		++iKillLogCnt;

		//킬로그 제거 Timer
		FTimerDelegate RemoveKillLogDelegate = FTimerDelegate::CreateUObject(this,
			&AGamePlayPC::ShowEndKillLog, iCurrentHandlerIdx);

		GetWorldTimerManager().SetTimer(KillLogTimerHandler[iCurrentHandlerIdx],
			RemoveKillLogDelegate,
			iKillLogRemoveSpeed,
			false);

		iCurrentHandlerIdx = (iCurrentHandlerIdx + 1) % iMaxKillLogCnt;
	}
}


void AGamePlayPC::UpdateOccupiedAreaRate(FString OccupiedAreaName, float Rate)
{
	if (PlayerMainWidget)
	{
		PlayerMainWidget->SetOccupiedRate(OccupiedAreaName, Rate);
	}
}

void AGamePlayPC::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGamePlayPC, iPlayerNumberID);
	DOREPLIFETIME(AGamePlayPC, PlayerTeam);
	DOREPLIFETIME(AGamePlayPC, PlayerName);
}

void AGamePlayPC::SingleModeUIOnEffect()
{
	if (OpacityValue > 0)
	{
		OpacityValue -= 0.1;
		PlayerMainWidget->SetSingleOpacity(OpacityValue);
		GetWorldTimerManager().SetTimer(OpacityHandler,
			this,
			&AGamePlayPC::SingleModeUIOnEffect,
			0.1f,
			false);
	}
	else
	{
		GetWorldTimerManager().ClearTimer(OpacityHandler);
		PlayerMainWidget->SetSingleUIVisibility(false);
		OpacityValue = 1.0f;
	}
}

void AGamePlayPC::RepeatModeUIOnEffect()
{
	if (OpacityValue > 0)
	{
		OpacityValue -= 0.1;
		PlayerMainWidget->SetRepeatOpacity(OpacityValue);
		GetWorldTimerManager().SetTimer(OpacityHandler,
			this,
			&AGamePlayPC::RepeatModeUIOnEffect,
			0.1f,
			false);
	}
	else
	{
		GetWorldTimerManager().ClearTimer(OpacityHandler);
		PlayerMainWidget->SetRepeatUIVisibility(false);
		OpacityValue = 1.0f;
	}
}

void AGamePlayPC::SetRespawnRemainTime()
{
	if (CurrentRate >= 1.0f)
	{
		CurrentRate = 0.0f;
		GetWorldTimerManager().ClearTimer(RespawnTimeHandler);
		PlayerMainWidget->SetSpawnUIVisibility(false);
	}
	else
	{
		CurrentRate += IncreaseRate;
		if (PlayerMainWidget)
		{
			PlayerMainWidget->SetSpawnWaitBarRate(CurrentRate);
		}
		GetWorldTimerManager().SetTimer(
			RespawnTimeHandler,
			this,
			&AGamePlayPC::SetRespawnRemainTime,
			IncreaseRate * ReSpawnWaitTime,
			false);
	}
}

void AGamePlayPC::SetGameStartRemainTime()
{
	if (iCurrentRemainTime > 0)
	{
		//시간 update
		--iCurrentRemainTime;

		//UI 갱신
		PlayerMainWidget->SetCountDown(iCurrentRemainTime);

		GetWorldTimerManager().SetTimer(
			GameStartTimeHandler,
			this,
			&AGamePlayPC::SetGameStartRemainTime,
			1.0f,
			false);
	}
	else
	{
		GetWorldTimerManager().ClearTimer(GameStartTimeHandler);
		PlayerMainWidget->SetCountDownUIVisibility(false);
	}
}

void AGamePlayPC::ShowEndKillLog(int iIdx)
{
	GetWorldTimerManager().ClearTimer(KillLogTimerHandler[iIdx]);

	if (PlayerMainWidget)
	{
		PlayerMainWidget->RemoveFirstKillLog();
	}

	iFirstHandlerIdx = (iFirstHandlerIdx + 1) % iMaxKillLogCnt;
	--iKillLogCnt;
}
