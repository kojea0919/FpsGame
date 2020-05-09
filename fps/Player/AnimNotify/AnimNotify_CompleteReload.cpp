// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_CompleteReload.h"
#include "Components/SkeletalMeshComponent.h"
#include "Game/GamePlay/GamePlayPC.h"
#include "Player/FpsPlayer.h"
#include "Weapon/Ar.h"

void UAnimNotify_CompleteReload::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	AFpsPlayer * pPlayer = Cast<AFpsPlayer>(MeshComp->GetOwner());
	if (nullptr == pPlayer)
	{
		UE_LOG(LogClass, Warning, TEXT("UAnimNotify_CompleteReload::Notify / pPlayer nullptr"));
	}

	pPlayer->SetWeaponAttachToHandSight(true);

	pPlayer->CurPlayerState = EPlayerState::e_Normal;
	pPlayer->UpdateMoveSpeed();
	pPlayer->CtoS_SetPlayerState(EPlayerState::e_Normal);

	AAr * pMyAr = pPlayer->pAr;
	pMyAr->UpdateBulletCnt();


	AGamePlayPC * pPc = Cast<AGamePlayPC>(pPlayer->GetController());
	if (nullptr == pPc)
	{
		UE_LOG(LogClass, Warning, TEXT("AFpsPlayer::Fire / pPc nullptr"));
		return;
	}
	pPc->UpdateRemainBulletUI(pMyAr->GetCurBulletCnt(), pMyAr->GetAllBulletCnt());

}
