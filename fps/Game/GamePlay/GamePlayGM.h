// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GamePlayGM.generated.h"

/**
 * 
 */

enum class EPlayerTeamState : uint8;

UCLASS()
class FPS_API AGamePlayGM : public AGameModeBase
{
	GENERATED_BODY()
public:
	AGamePlayGM();

	virtual void PostLogin(APlayerController* NewPlayer) override;

	//bIsRedTeam : 죽은 플레이어의 팀이 Red이면 true
	void UpdateKillScore(bool bIsRedTeam);

	EPlayerTeamState IsRedTeam(int iPlayerNumberID);

	//bIsRedteam : 점령 지역을 점령한 팀이 Red면 true
	void IncreaseOccupiedSocre(bool bIsRedTeam);
	void DecreaseOccupiedSocre(bool bIsRedTeam);


	//준비된 플레이어 수를 증가시키고
	//모두 준비되면 게임 시작 Count Down
	void AddReadyPlayerNum();


	//플레이어가 죽으면 UI Update시키고
	//리스폰 대기시간만큼 기다렸다 다시 살리기
	void PlayerDead(int iKillPlayerNumberID, int iDeathPlayerNumberID);


	int GetMaxPlayerNum() { return iMaxPlayerNum; }

	int GetPlayerNumberID() { return ++iCurrentPlayerNum; }

private:
	int iCurrentPlayerNum = 0;
	int iMaxPlayerNum = 6;

	//Kill Score
	//--------------------------------------
	int iRedTeamKillScore = 0;
	int iBlueTeamKillScore = 0;
	int iWinKillScore = 15;
	//--------------------------------------

	//Occupied Score
	//--------------------------------------
	int iRedTeamOccupiedScore = 0;
	int iBlueTeamOccupiedScore = 0;
	int iWinOccupiedScore = 3;
	//--------------------------------------

	//플레이어 정보
	//--------------------------------------
	TArray<class AGamePlayPC*> FpsPlayerList;
	TArray<FVector> FpsPlayerSpawnLocation;
	//--------------------------------------

	//시작 가능한 플레이어 수
	//--------------------------------------
	int iReadyPlayerNum = 0;
	//--------------------------------------

	//Game Start Spawn대기 시간
	//--------------------------------------
	int iSpawnTime = 5.0f;
	FTimerHandle SpawnWaitHandler;

	void SetPlayerStartLocation();
	//--------------------------------------


	//죽고나서 Respawn대기 시간
	//--------------------------------------
	int iRespwanTime = 5.0f;
	FTimerHandle RespawnWaitHandler[6];

	void SetPlayerRespawn(int iPlayerNumberID);
	//--------------------------------------


	//Team Set Complete
	//--------------------------------------
	void TeamSetComplete();
	//--------------------------------------


	//게임이 끝났을 때 호출할 함수
	//--------------------------------------
	//bIsRedTeamWin = ture : red 승리
	//bIsRedTeamWin = false : blue 승리
	void GameEnd(bool bIsRedTeamWin);
	//--------------------------------------
};
