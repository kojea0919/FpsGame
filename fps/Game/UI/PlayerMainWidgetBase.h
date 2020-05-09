// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerMainWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API UPlayerMainWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	//체력 ProgressBar
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UProgressBar * HPBar;

	//남은 총알 갯수
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UTextBlock * BulletCnt;

	//단발Text
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UTextBlock * SingleText;

	//연발Text
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UTextBlock * RepeatText;

	//Red Team Text
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UTextBlock * RedTeamText;

	//Blue Team Text
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UTextBlock * BlueTeamText;

	//TeamColor Image
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UImage * TeamColorImage;

	//각 팀 킬점수
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UTextBlock * RedTeamKillScore;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UTextBlock * BlueTeamKillScore;

	//플레이어 스폰 중Text, 스폰 대기 ProgressBar
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UTextBlock * SpawnText;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UProgressBar * SpawnWait_Bar;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UTextBlock * CountDownText;


	//점령 지역 상태 표시 Text, ProgressBar
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UProgressBar * OccupiedArear_A_Bar;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UProgressBar * OccupiedArear_B_Bar;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UProgressBar * OccupiedArear_C_Bar;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UVerticalBox * KillLogBox;

	//---------------------------------------------------------



	//체력바 게이지 Setting
	void SetHPBarRate(float Rate);

	float GetHPBarRate();

	//남은 총알 수 Setting
	void UpdateRemainBulletCnt(int iCurBulletCnt,int iAllBulletCnt);

	//발사 모드 
	//-----------------------------------
	void SetSingleUIVisibility(bool bOn);
	void SetRepeatUIVisibility(bool bOn);

	void SetSingleOpacity(float Value);
	void SetRepeatOpacity(float Value);
	//-----------------------------------


	//Update Kill Score
	//-----------------------------------
	void UpdateRedTeamKillScore(int iNewScore);
	void UpdateBlueTeamKillScore(int iNewScore);
	//-----------------------------------


	//Spawn관련 함수
	//-----------------------------------
	void SetSpawnUIVisibility(bool bOn);
	void SetSpawnWaitBarRate(float Rate);
	//-----------------------------------


	//CountDown관련 함수
	//-----------------------------------
	void SetCountDownUIVisibility(bool bOn);
	void SetCountDown(int iCountNum);
	//-----------------------------------

	//Team
	//-----------------------------------
	void SetTeam(bool bRedTeam);
	//-----------------------------------

	//점령 Rate
	//-----------------------------------
	void SetOccupiedRate(FString OccupiedName, float Rate);
	//-----------------------------------


	//Kill Log 생성
	//-----------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UUserWidget> KillLogWidgetClass;

	void CreateNewKillLog(FText KillPlayerName, FText DeathPlayerName);
	void RemoveFirstKillLog();


	TArray<UUserWidget*> KillLogArray;

	//-----------------------------------
};
