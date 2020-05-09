// Fill out your copyright notice in the Description page of Project Settings.


#include "OccupiedArea.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "GamePlay/OccupiedArea/OccupedNameWidgetBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "UnrealNetwork.h"
#include "Game/GamePlay/GamePlayPC.h"
#include "Game/GamePlay/GamePlayGM.h"

void AOccupiedArea::OnRep_ReplicateOccupiedScore()
{
	fOccupiedScore = ReplicateOccupiedScore;
}

// Sets default values
AOccupiedArea::AOccupiedArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	OcuupiedAreaBox = CreateDefaultSubobject<UBoxComponent>(TEXT("OcuupiedAreaBox"));
	OcuupiedAreaBox->SetCollisionProfileName(TEXT("OccupiedArea"));
	RootComponent = OcuupiedAreaBox;
	OcuupiedAreaBox->OnComponentBeginOverlap.AddDynamic(this, &AOccupiedArea::DoActorBeginOverlap);
	OcuupiedAreaBox->OnComponentEndOverlap.AddDynamic(this, &AOccupiedArea::DoActorEndOverlap);
	

	OccupiedAreaNameWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OccupiedAreaNameWidget"));
	OccupiedAreaNameWidget->SetupAttachment(OcuupiedAreaBox);

}

// Called when the game starts or when spawned
void AOccupiedArea::BeginPlay()
{
	Super::BeginPlay();

	UOccupedNameWidgetBase * OccupiedNameWidget = Cast<UOccupedNameWidgetBase>(OccupiedAreaNameWidget->GetUserWidgetObject());
	if (OccupiedNameWidget)
	{
		OccupiedNameWidget->SetOccupiedName(OccupiedAreaName);
	}
}

// Called every frame
void AOccupiedArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if(GetWorld()->IsServer())
		CheckOccupiedState(DeltaTime);

	//���� ���� ProgressBar Update
	UpdateProgressBar(DeltaTime);
}

void AOccupiedArea::OnRep_CurOccupiedState()
{
	UOccupedNameWidgetBase * OccupiedNameWidget = Cast<UOccupedNameWidgetBase>(OccupiedAreaNameWidget->GetUserWidgetObject());
	if (!OccupiedNameWidget)
	{
		return;
	}

	if (CurOccupiedState == EOccupiedState::e_NotOccupied)
	{
		OccupiedNameWidget->SetOcuupiedInitColor();
	}
	else if (CurOccupiedState == EOccupiedState::e_RedTeamOccupied)
	{
		OccupiedNameWidget->SetOccupiedTeamColor(true);
	}
	else
	{
		OccupiedNameWidget->SetOccupiedTeamColor(false);
	}
}

void AOccupiedArea::Client_ComputeOccupiedScore(float AddScore, EOccupiedState OcuupiedStateOfThisLoop)
{
	//1. ���ɵ��� ���� ����
	if (CurOccupiedState == EOccupiedState::e_NotOccupied)
	{
		fOccupiedScore += AddScore;
	}
	//2. ������ ���� ���� ���� ��
	else if (CurOccupiedState == OcuupiedStateOfThisLoop)
	{
		fOccupiedScore += AddScore;
		return;
	}
	//3. �ٸ� ���� ���� ��
	else if (CurOccupiedState != OcuupiedStateOfThisLoop)
	{
		fOccupiedScore -= AddScore;
		if (fOccupiedScore > 0)
		{
			return;
		}
		fOccupiedScore = 0.0f;
	}
	fOccupiedScore = fOccupiedScore < fMaxScore ? fOccupiedScore : fMaxScore;
}

void AOccupiedArea::Server_ComputeOccupiedScore(float AddScore, EOccupiedState OcuupiedStateOfThisLoop)
{
	//1. ���ɵ��� ���� ����
	if (CurOccupiedState == EOccupiedState::e_NotOccupied)
	{
		//���� �켼�� ������ ���� Set
		CurOccupiedState = OcuupiedStateOfThisLoop;	
		OnRep_CurOccupiedState();

		//Score �߰�
		fOccupiedScore += AddScore;
	}
	//2. ������ ���� ���� ���� ��
	else if (CurOccupiedState == OcuupiedStateOfThisLoop)
	{
		//Scroe �߰�
		fOccupiedScore += AddScore;
	}
	//3. ���ݱ��� �����ߴ� ���� �ٸ� ���� ���� ��
	else if (CurOccupiedState != OcuupiedStateOfThisLoop)
	{
		//���������� �������� ���� �ִ� ���
		if (fOccupiedScore >= fMaxScore)
		{
			//�������� ����
			AGamePlayGM * pGm = Cast<AGamePlayGM>(UGameplayStatics::GetGameMode(GetWorld()));
			if (!pGm)
			{
				UE_LOG(LogClass, Warning, TEXT("AOccupiedArea::Server_ComputeOccupiedScore / !pGm"));
				return;
			}
			if (CurOccupiedState == EOccupiedState::e_RedTeamOccupied)
			{
				pGm->DecreaseOccupiedSocre(true);
			}
			else if (CurOccupiedState == EOccupiedState::e_BlueTeamOccupied)
			{
				pGm->DecreaseOccupiedSocre(false);
			}
		}


		//Score ����
		fOccupiedScore -= AddScore;
		if (fOccupiedScore > 0)
		{
			return;
		}

		//Score�� �����Ǵ� ���
		CurOccupiedState = EOccupiedState::e_NotOccupied;
		OnRep_CurOccupiedState();

		fOccupiedScore = 0.0f;
	}

	//���� �Ϸ��
	if (fOccupiedScore >= fMaxScore)
	{
		fOccupiedScore = fMaxScore;

		AGamePlayGM * pGm = Cast<AGamePlayGM>(UGameplayStatics::GetGameMode(GetWorld()));
		if (!pGm)
		{
			UE_LOG(LogClass, Warning, TEXT("AOccupiedArea::Server_ComputeOccupiedScore / !pGm"));
			return;
		}

		if (CurOccupiedState == EOccupiedState::e_RedTeamOccupied)
		{
			pGm->IncreaseOccupiedSocre(true);
		}
		else if (CurOccupiedState == EOccupiedState::e_BlueTeamOccupied)
		{
			pGm->IncreaseOccupiedSocre(false);
		}
	}
}

float AOccupiedArea::ComputeAddScore(float DeltaTime, EOccupiedState & OcuupiedStateOfThisLoop)
{
	float fAddScore;

	//Red ���ڰ� ���� ���
	if (iRedTeamCnt > iBlueTeamCnt)
	{
		OcuupiedStateOfThisLoop = EOccupiedState::e_RedTeamOccupied;
		fAddScore = iRedTeamCnt * DeltaTime;
	}
	//Blue ���ڰ� ���� ���
	else if (iBlueTeamCnt > iRedTeamCnt)
	{
		OcuupiedStateOfThisLoop = EOccupiedState::e_BlueTeamOccupied;
		fAddScore = iBlueTeamCnt * DeltaTime;
	}
	//���� ���
	else
	{
		OcuupiedStateOfThisLoop = EOccupiedState::e_NotOccupied;
		fAddScore = 0.0f;
	}

	//�� ���ִ� ���
	if (OcuupiedStateOfThisLoop == CurOccupiedState && fOccupiedScore >= fMaxScore)
		return 0.0f;

	return fAddScore * fOccupiedSpeed;
}

void AOccupiedArea::UpdateProgressBar(float DeltaTime)
{
	//������ �ƴ� ��� �����ؾ��� ���� Client������� ���
	float fAddScore;
	EOccupiedState OcuupiedStateOfThisLoop;
	if (!GetWorld()->IsServer())
	{
		fAddScore = ComputeAddScore(DeltaTime, OcuupiedStateOfThisLoop);
		if (fAddScore == 0)
			return;

		Client_ComputeOccupiedScore(fAddScore, OcuupiedStateOfThisLoop);
	}

	AGamePlayPC * pPc = Cast<AGamePlayPC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!pPc)
	{
		UE_LOG(LogClass, Warning, TEXT("AOccupiedArea::UpdateProgressBar / !pPc"));
		return;
	}

	pPc->UpdateOccupiedAreaRate(OccupiedAreaName.ToString(), fOccupiedScore / fMaxScore);
}

void AOccupiedArea::CheckOccupiedState(float DeltaTime)
{
	//���� �÷��̾�� Counting X
	int iLen = OverlapedPlayer.Num();
	for (int iCnt = 0; iCnt < iLen; ++iCnt)
	{
		if (OverlapedPlayer[iCnt]->IsDead())
		{
			if (OverlapedPlayer[iCnt]->IsRedTeam())
			{
				--iRedTeamCnt;
			}
			else
			{
				--iBlueTeamCnt;
			}
		}
	}

	//���� ���� �������� �ִ� �÷��̾� ���� ���� �߰� ���� ���
	EOccupiedState OcuupiedStateOfThisLoop;
	float fAddScore;
	fAddScore = ComputeAddScore(DeltaTime, OcuupiedStateOfThisLoop);
	if (fAddScore == 0)
		return;

	//���� �߰�
	Server_ComputeOccupiedScore(fAddScore, OcuupiedStateOfThisLoop);
	
}


void AOccupiedArea::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AOccupiedArea, CurOccupiedState);
	DOREPLIFETIME(AOccupiedArea, iRedTeamCnt);
	DOREPLIFETIME(AOccupiedArea, iBlueTeamCnt);
	DOREPLIFETIME(AOccupiedArea, ReplicateOccupiedScore);
}

void AOccupiedArea::SetOccupiedAreaBoxNoCollision()
{
	OcuupiedAreaBox->SetCollisionProfileName(TEXT("NoCollision"));
}

void AOccupiedArea::DoActorBeginOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (GetWorld()->IsServer())
	{
		AFpsPlayer * pCurPlayer = Cast<AFpsPlayer>(OtherActor);
		if (!pCurPlayer)
		{
			UE_LOG(LogClass, Warning, TEXT(" AOccupiedArea::DoActorBeginOverlap / !pCurPlayer"));
			return;
		}

		if (INDEX_NONE == OverlapedPlayer.Find(pCurPlayer))
		{
			OverlapedPlayer.Add(pCurPlayer);
			
			if (pCurPlayer->IsRedTeam())
			{
				++iRedTeamCnt;
			}
			else
			{
				++iBlueTeamCnt;
			}
			ReplicateOccupiedScore = fOccupiedScore;
		}
	}
}

void AOccupiedArea::DoActorEndOverlap(UPrimitiveComponent * OevrlapeedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (GetWorld()->IsServer())
	{
		AFpsPlayer * pCurPlayer = Cast<AFpsPlayer>(OtherActor);
		if (!pCurPlayer)
		{
			UE_LOG(LogClass, Warning, TEXT(" AOccupiedArea::DoActorBeginOverlap / !pCurPlayer"));
			return;
		}

		if (INDEX_NONE != OverlapedPlayer.Find(pCurPlayer))
		{
			OverlapedPlayer.Remove(pCurPlayer);

			if (pCurPlayer->IsRedTeam())
			{
				--iRedTeamCnt;
			}
			else
			{
				--iBlueTeamCnt;
			}
			ReplicateOccupiedScore = fOccupiedScore;
		}
	}

}
