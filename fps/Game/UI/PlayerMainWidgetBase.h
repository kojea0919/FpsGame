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

	//ü�� ProgressBar
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UProgressBar * HPBar;

	//���� �Ѿ� ����
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UTextBlock * BulletCnt;

	//�ܹ�Text
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UTextBlock * SingleText;

	//����Text
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

	//�� �� ų����
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UTextBlock * RedTeamKillScore;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UTextBlock * BlueTeamKillScore;

	//�÷��̾� ���� ��Text, ���� ��� ProgressBar
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UTextBlock * SpawnText;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UProgressBar * SpawnWait_Bar;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UTextBlock * CountDownText;


	//���� ���� ���� ǥ�� Text, ProgressBar
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UProgressBar * OccupiedArear_A_Bar;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UProgressBar * OccupiedArear_B_Bar;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UProgressBar * OccupiedArear_C_Bar;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UVerticalBox * KillLogBox;

	//---------------------------------------------------------



	//ü�¹� ������ Setting
	void SetHPBarRate(float Rate);

	float GetHPBarRate();

	//���� �Ѿ� �� Setting
	void UpdateRemainBulletCnt(int iCurBulletCnt,int iAllBulletCnt);

	//�߻� ��� 
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


	//Spawn���� �Լ�
	//-----------------------------------
	void SetSpawnUIVisibility(bool bOn);
	void SetSpawnWaitBarRate(float Rate);
	//-----------------------------------


	//CountDown���� �Լ�
	//-----------------------------------
	void SetCountDownUIVisibility(bool bOn);
	void SetCountDown(int iCountNum);
	//-----------------------------------

	//Team
	//-----------------------------------
	void SetTeam(bool bRedTeam);
	//-----------------------------------

	//���� Rate
	//-----------------------------------
	void SetOccupiedRate(FString OccupiedName, float Rate);
	//-----------------------------------


	//Kill Log ����
	//-----------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UUserWidget> KillLogWidgetClass;

	void CreateNewKillLog(FText KillPlayerName, FText DeathPlayerName);
	void RemoveFirstKillLog();


	TArray<UUserWidget*> KillLogArray;

	//-----------------------------------
};
