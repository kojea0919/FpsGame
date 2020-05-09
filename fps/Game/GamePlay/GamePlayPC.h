// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GamePlayPC.generated.h"

/**
 * 
 */


UENUM(BlueprintType)
enum class EPlayerTeamState : uint8
{
	e_NotSet = 0		UMETA(DisplayName = "NotSet"),
	e_RedTeam = 1		UMETA(DisplayName = "RedTeam"),
	e_BlueTeam = 2		UMETA(DisplayName = "BlueTeam"),
};


UCLASS()
class FPS_API AGamePlayPC : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	//UI
	//----------------------------------------------------------------
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UPlayerMainWidgetBase * PlayerMainWidget;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<class UPlayerMainWidgetBase> PlayerMainWidgetClass;

	//--------

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UGameEndWidgetBase * GameEndWidget;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<class UGameEndWidgetBase> GameEndWidgetClass;
	//----------------------------------------------------------------


	//HPBar Setting
	//----------------------------------------------------------------
	void SetHPBar(float Rate);
	//----------------------------------------------------------------

	//SetInputMode
	//----------------------------------------------------------------
	void SetOnlyUI(bool bIsOnlyUI);
	//----------------------------------------------------------------

	//남은 총알 수 Update
	//----------------------------------------------------------------
	void UpdateRemainBulletUI(int iCurBulletCnt, int iAllBulletCnt);
	//----------------------------------------------------------------

	//단발or연발로 변경시 UI On
	//----------------------------------------------------------------
	void ShowSingleUI();
	void ShowRepeatUI();
	//----------------------------------------------------------------


	//플레이어 팀 
	//----------------------------------------------------------------
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, ReplicatedUsing = "OnRep_PlayerTeam")
	EPlayerTeamState PlayerTeam = EPlayerTeamState::e_NotSet;

	UFUNCTION()
	void OnRep_PlayerTeam();

	bool IsRedTeam();
	//----------------------------------------------------------------


	//플레이어 식별
	//----------------------------------------------------------------
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Replicated)
	int iPlayerNumberID;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Replicated)
	FText PlayerName;

	UFUNCTION(Client,Reliable)
	void StoC_GameEnd(bool bIsRedTeamWin);
	void StoC_GameEnd_Implementation(bool bIsRedTeamWin);


	UFUNCTION(Server, Reliable)
	void CtoS_SetTeam();
	void CtoS_SetTeam_Implementation();

	UFUNCTION(Server,Reliable)
	void CtoS_SetPlayerID(const FText & PlayerID);
	void CtoS_SetPlayerID_Implementation(const FText & PlayerID);
	//----------------------------------------------------------------

	//게임 시작 타이머 Start
	//-------------------------------
	UFUNCTION(Server, Reliable)
	void CtoS_ReadyComplete();
	void CtoS_ReadyComplete_Implementation();

	UFUNCTION(Client,Reliable)
	void StoC_GameStartTimerOn(int iSpawnTime);
	void StoC_GameStartTimerOn_Implementation(int iSpawnTime);
	//-------------------------------
	
	//Respawn 타이머 Start
	//-------------------------------
	UFUNCTION(Client, Reliable)
	void StoC_RespawnStartTimerOn(int iSpawnTime);
	void StoC_RespawnStartTimerOn_Implementation(int iSpawnTime);
	//-------------------------------


	//KillLog 생성 & 제거
	//-------------------------------
	UFUNCTION(Client, Reliable)
	void StoC_ShowKillLog(const FText & KillPlayerID, const FText & DeathPlayerID);
	void StoC_ShowKillLog_Implementation(const FText & KillPlayerID, const FText & DeathPlayerID);
	//-------------------------------



	void UpdateOccupiedAreaRate(FString OccupiedAreaName, float Rate);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	


private:

	//연발 or 단발 UI Fade Out
	//-------------------------------
	FTimerHandle OpacityHandler;
	float OpacityValue = 1.0f;
	void SingleModeUIOnEffect();
	void RepeatModeUIOnEffect();
	//-------------------------------


	//리스폰 대기 시간 표시
	//-------------------------------
	FTimerHandle RespawnTimeHandler;
	void SetRespawnRemainTime();
	float CurrentRate = 0.0;
	float IncreaseRate = 0.05f;
	int ReSpawnWaitTime;
	//-------------------------------


	//게임시작 대기 시간 표시
	//-------------------------------
	FTimerHandle GameStartTimeHandler;
	int iCurrentRemainTime;
	void SetGameStartRemainTime();
	//-------------------------------


	//킬로그 제거
	//-------------------------------
	FTimerHandle KillLogTimerHandler[4];
	int iKillLogCnt = 0;
	int iMaxKillLogCnt = 4;

	int iKillLogRemoveSpeed = 10;

	int iFirstHandlerIdx = 0;
	int iCurrentHandlerIdx = 0;

	void ShowEndKillLog(int iIdx);
	//-------------------------------
};
