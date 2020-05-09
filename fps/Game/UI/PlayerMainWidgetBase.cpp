// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMainWidgetBase.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/VerticalBox.h"
#include "Kismet/GameplayStatics.h"
#include "Game/GamePlay/GamePlayPC.h"
#include "Game/UI/KillLogWidgetBase.h"

void UPlayerMainWidgetBase::NativeConstruct()
{
	HPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HP_Bar")));
	
	BulletCnt = Cast<UTextBlock>(GetWidgetFromName(TEXT("BulletCnt")));

	SingleText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Single")));
	RepeatText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Repeat")));

	RedTeamText = Cast<UTextBlock>(GetWidgetFromName(TEXT("RedTeam")));

	BlueTeamText = Cast<UTextBlock>(GetWidgetFromName(TEXT("BlueTeam")));

	TeamColorImage = Cast<UImage>(GetWidgetFromName(TEXT("TeamColor")));

	RedTeamKillScore = Cast<UTextBlock>(GetWidgetFromName(TEXT("RedTeamKillScore")));
	BlueTeamKillScore = Cast<UTextBlock>(GetWidgetFromName(TEXT("BlueTeamKillScore")));

	SpawnText = Cast<UTextBlock>(GetWidgetFromName(TEXT("SpawnText")));
	SpawnWait_Bar = Cast<UProgressBar>(GetWidgetFromName(TEXT("SpawnWait_Bar")));

	CountDownText = Cast<UTextBlock>(GetWidgetFromName(TEXT("StartCountDown")));

	OccupiedArear_A_Bar = Cast<UProgressBar>(GetWidgetFromName(TEXT("OccupiedArear_A_Bar")));
	OccupiedArear_B_Bar = Cast<UProgressBar>(GetWidgetFromName(TEXT("OccupiedArear_B_Bar")));
	OccupiedArear_C_Bar = Cast<UProgressBar>(GetWidgetFromName(TEXT("OccupiedArear_C_Bar")));

	KillLogBox = Cast<UVerticalBox>(GetWidgetFromName(TEXT("KillLogBox")));

	//----------------------------------------------------------------------------

	FString RemainBulletCnt = FString(TEXT("30 / 150"));
	BulletCnt->SetText(FText::FromString(RemainBulletCnt));

	SingleText->SetVisibility(ESlateVisibility::Hidden);
	RepeatText->SetVisibility(ESlateVisibility::Hidden);

	RedTeamText->SetVisibility(ESlateVisibility::Hidden);
	BlueTeamText->SetVisibility(ESlateVisibility::Hidden);

	SpawnText->SetVisibility(ESlateVisibility::Hidden);
	SpawnWait_Bar->SetVisibility(ESlateVisibility::Hidden);

	//----------------------------------------------------------------------------
}

void UPlayerMainWidgetBase::SetHPBarRate(float Rate)
{
	if (HPBar)
	{
		HPBar->SetPercent(Rate);
	}
}

float UPlayerMainWidgetBase::GetHPBarRate()
{
	return HPBar->Percent;
}

void UPlayerMainWidgetBase::UpdateRemainBulletCnt(int iCurBulletCnt, int iAllBulletCnt)
{
	FString RemainBulletCnt;
	
	//ÇöÀç ÅºÃ¢ ÃÑ¾Ë °¹¼ö Update
	//------------------------------------------------------
	if (iCurBulletCnt < 10)
	{
		RemainBulletCnt.Append(TEXT("0"));
	}
	RemainBulletCnt.Append(FString::FromInt(iCurBulletCnt));
	//------------------------------------------------------

	RemainBulletCnt.Append(TEXT(" / "));

	//ÃÑ ÅºÃ¢ ÃÑ¾Ë °¹¼ö Update
	//------------------------------------------------------
	if (iAllBulletCnt < 10)
	{
		RemainBulletCnt.Append(TEXT("00"));
	}
	else if (iAllBulletCnt < 100)
	{
		RemainBulletCnt.Append(TEXT("0"));
	}
	RemainBulletCnt.Append(FString::FromInt(iAllBulletCnt));
	//------------------------------------------------------

	if(BulletCnt)
		BulletCnt->SetText(FText::FromString(RemainBulletCnt));
}

void UPlayerMainWidgetBase::SetSingleUIVisibility(bool bOn)
{
	if (!SingleText)
	{
		UE_LOG(LogClass, Warning, TEXT("UPlayerMainWidgetBase::SetSingleUIVisibility / !SingleText"));
	}

	if(bOn)
		SingleText->SetVisibility(ESlateVisibility::Visible);
	else
		SingleText->SetVisibility(ESlateVisibility::Hidden);
}

void UPlayerMainWidgetBase::SetRepeatUIVisibility(bool bOn)
{
	if (!RepeatText)
	{
		UE_LOG(LogClass, Warning, TEXT("UPlayerMainWidgetBase::SetRepeatUIVisibility / !RepeatText"));
	}

	if(bOn)
		RepeatText->SetVisibility(ESlateVisibility::Visible);
	else
		RepeatText->SetVisibility(ESlateVisibility::Hidden);
}

void UPlayerMainWidgetBase::SetSingleOpacity(float Value)
{
	if(SingleText)
		SingleText->SetOpacity(Value);
}

void UPlayerMainWidgetBase::SetRepeatOpacity(float Value)
{
	if(RepeatText)
		RepeatText->SetOpacity(Value);
}

void UPlayerMainWidgetBase::SetTeam(bool bRedTeam)
{
	if (!RedTeamText || !TeamColorImage)
	{
		UE_LOG(LogClass, Warning, TEXT("UPlayerMainWidgetBase::SetTeam / !RedTeamText or !TeamColorImage"));
		return;
	}

	if (bRedTeam)
	{
		RedTeamText->SetVisibility(ESlateVisibility::Visible);
		TeamColorImage->SetColorAndOpacity(FLinearColor(1.0f, 0.0f, 0.0f));
	}
	else
	{
		BlueTeamText->SetVisibility(ESlateVisibility::Visible);
		TeamColorImage->SetColorAndOpacity(FLinearColor(0.0f, 0.0f, 1.0f));
	}
}

void UPlayerMainWidgetBase::SetOccupiedRate(FString OccupiedName, float Rate)
{
	if (OccupiedName.Compare("A") == 0)
	{
		OccupiedArear_A_Bar->SetPercent(Rate);
	}
	else if (OccupiedName.Compare("B") == 0)
	{
		OccupiedArear_B_Bar->SetPercent(Rate);
	}
	else if (OccupiedName.Compare("C") == 0)
	{
		OccupiedArear_C_Bar->SetPercent(Rate);
	}
}

void UPlayerMainWidgetBase::CreateNewKillLog(FText KillPlayerName, FText DeathPlayerName)
{
	if (KillLogWidgetClass)
	{
		UKillLogWidgetBase * NewKillLog = Cast<UKillLogWidgetBase>(CreateWidget(GetWorld(), KillLogWidgetClass));
		NewKillLog->SetKillPlayerName(KillPlayerName);
		NewKillLog->SetDeathPlayerName(DeathPlayerName);


		KillLogArray.Add(NewKillLog);
		KillLogBox->AddChild(NewKillLog);
	}
}

void UPlayerMainWidgetBase::RemoveFirstKillLog()
{
	int iLen = KillLogArray.Num();
	if (iLen > 0)
	{
		UUserWidget * FirstKillLog = KillLogArray[0];
		KillLogArray.RemoveAt(0);
		FirstKillLog->RemoveFromParent();
	}
}


void UPlayerMainWidgetBase::UpdateRedTeamKillScore(int iNewScore)
{
	if(RedTeamKillScore)
		RedTeamKillScore->SetText(FText::FromString(FString::FromInt(iNewScore)));
}

void UPlayerMainWidgetBase::UpdateBlueTeamKillScore(int iNewScore)
{
	if(BlueTeamKillScore)
		BlueTeamKillScore->SetText(FText::FromString(FString::FromInt(iNewScore)));
}

void UPlayerMainWidgetBase::SetSpawnUIVisibility(bool bOn)
{
	if (!SpawnText || !SpawnWait_Bar)
	{
		UE_LOG(LogClass, Warning, TEXT("UPlayerMainWidgetBase::SetSpawnUIVisibility / !SpawnText"));
		return;
	}

	if (bOn)
	{
		SpawnText->SetVisibility(ESlateVisibility::Visible);
		SpawnWait_Bar->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		SpawnText->SetVisibility(ESlateVisibility::Hidden);
		SpawnWait_Bar->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UPlayerMainWidgetBase::SetSpawnWaitBarRate(float Rate)
{
	if (!SpawnWait_Bar)
	{
		UE_LOG(LogClass, Warning, TEXT("UPlayerMainWidgetBase::SetSpawnWaitBarRate / !SpawnWait_Bar"));
		return;
	}

	SpawnWait_Bar->SetPercent(Rate);
}

void UPlayerMainWidgetBase::SetCountDownUIVisibility(bool bOn)
{
	if(!CountDownText)
	{
		UE_LOG(LogClass, Warning, TEXT("UPlayerMainWidgetBase::SetCountDownUIVisibility / !CountDownText"));
		return;
	}

	if (bOn)
	{
		CountDownText->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		CountDownText->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UPlayerMainWidgetBase::SetCountDown(int iCountNum)
{
	if (!CountDownText)
	{
		UE_LOG(LogClass, Warning, TEXT("UPlayerMainWidgetBase::SetCountDownUIVisibility / !SetCountDown"));
		return;
	}

	CountDownText->SetText(FText::FromString(FString::FromInt(iCountNum)));
}

