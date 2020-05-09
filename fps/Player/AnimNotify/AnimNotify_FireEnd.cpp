// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_FireEnd.h"
#include "Components/SkeletalMeshComponent.h"
#include "Player/FpsPlayer.h"

void UAnimNotify_FireEnd::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	AFpsPlayer * pPlayer = Cast<AFpsPlayer>(MeshComp->GetOwner());
	if (nullptr == pPlayer)
	{
		UE_LOG(LogClass, Warning, TEXT("UAnimNotify_FireEnd::Notify / pPlayer nullptr"));
		return;
	}
	pPlayer->bIsFire = false;
	pPlayer->CtoS_SetPlayerIsFire(false);
}
