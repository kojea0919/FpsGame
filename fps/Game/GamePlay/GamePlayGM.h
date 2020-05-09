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

	//bIsRedTeam : ���� �÷��̾��� ���� Red�̸� true
	void UpdateKillScore(bool bIsRedTeam);

	EPlayerTeamState IsRedTeam(int iPlayerNumberID);

	//bIsRedteam : ���� ������ ������ ���� Red�� true
	void IncreaseOccupiedSocre(bool bIsRedTeam);
	void DecreaseOccupiedSocre(bool bIsRedTeam);


	//�غ�� �÷��̾� ���� ������Ű��
	//��� �غ�Ǹ� ���� ���� Count Down
	void AddReadyPlayerNum();


	//�÷��̾ ������ UI Update��Ű��
	//������ ���ð���ŭ ��ٷȴ� �ٽ� �츮��
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

	//�÷��̾� ����
	//--------------------------------------
	TArray<class AGamePlayPC*> FpsPlayerList;
	TArray<FVector> FpsPlayerSpawnLocation;
	//--------------------------------------

	//���� ������ �÷��̾� ��
	//--------------------------------------
	int iReadyPlayerNum = 0;
	//--------------------------------------

	//Game Start Spawn��� �ð�
	//--------------------------------------
	int iSpawnTime = 5.0f;
	FTimerHandle SpawnWaitHandler;

	void SetPlayerStartLocation();
	//--------------------------------------


	//�װ��� Respawn��� �ð�
	//--------------------------------------
	int iRespwanTime = 5.0f;
	FTimerHandle RespawnWaitHandler[6];

	void SetPlayerRespawn(int iPlayerNumberID);
	//--------------------------------------


	//Team Set Complete
	//--------------------------------------
	void TeamSetComplete();
	//--------------------------------------


	//������ ������ �� ȣ���� �Լ�
	//--------------------------------------
	//bIsRedTeamWin = ture : red �¸�
	//bIsRedTeamWin = false : blue �¸�
	void GameEnd(bool bIsRedTeamWin);
	//--------------------------------------
};
