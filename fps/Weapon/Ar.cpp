// Fill out your copyright notice in the Description page of Project Settings.


#include "Ar.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Sound/SoundBase.h"
#include "Particles/ParticleSystemComponent.h"
#include "Bullet/ArBullet/ArBullet.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "UnrealNetwork.h"
#include "Player/FpsPlayer.h"
#include "Game/GamePlay/GamePlayPC.h"

void AAr::CtoS_Shoot_Implementation()
{
	AFpsPlayer * pPlayer = Cast<AFpsPlayer>(GetOwner());
	if (!pPlayer)
	{
		UE_LOG(LogClass, Warning, TEXT("AAr::CtoS_Shoot_Implementation / !pPlayer"));
		return;
	}

	//남은 총알이 없으면 pass
	if (iCurBulletCnt == 0)
	{
		StoA_SpawnNoBulletSound();

		if (pPlayer->bIsFire && CurShootMode == EShootMode::e_Repeater)
		{
			GetWorldTimerManager().ClearTimer(RepeaterTimerHandle);
			pPlayer->bIsFire = false;
		}

		return;
	}


	//남은 총알이 있는 경우
	SpawnBullet();

	//Update BulletCnt
	--iCurBulletCnt;
	OnRep_BulletCnt();


	//연발일 경우 Timer에 등록
	if (CurShootMode == EShootMode::e_Repeater)
	{
		UE_LOG(LogClass, Warning, TEXT("call"));
		GetWorldTimerManager().SetTimer(
			RepeaterTimerHandle,
			this,
			&AAr::CtoS_Shoot,
			0.11f);
	}
	pPlayer->bIsFire = true;

	//총기 반동 Update
	UpdateCurrentReboundRate(false);

	pPlayer->StoC_ApplyWeaponRebound(CurrentReboundRate);
}

void AAr::SpawnBullet()
{
	if (!GetWorld()->IsServer())
		return;


	FTransform MuzzleTransform = WeaponBody->GetSocketTransform(TEXT("MuzzleSocket"));
	AArBullet * pBullet = GetWorld()->SpawnActor<AArBullet>(ArBullet_Template, MuzzleTransform);
	if (!pBullet)
	{
		UE_LOG(LogClass, Warning, TEXT("AAr::SpawnBullet / !pBullet"));
		return;
	}

	AFpsPlayer * pPlayer = Cast<AFpsPlayer>(GetOwner());
	if (!pPlayer)
	{
		UE_LOG(LogClass, Warning, TEXT("AAr::SpawnBullet / !pPlayer"));
		return;
	}
	pBullet->SetOwner(pPlayer);

	if (pPlayer->IsRedTeam())
		pBullet->bIsRedTeamBullet = true;
	else
		pBullet->bIsRedTeamBullet = false;

	//Muzzle Flash & Sound
	StoA_SpawnMuzzleFlashAndFireSound();
}

void AAr::CtoS_StopShoot_Implementation()
{
	GetWorldTimerManager().ClearTimer(RepeaterTimerHandle);

	//총기 반동 초기화
	UpdateCurrentReboundRate(true);

	AFpsPlayer * pPlayer = Cast<AFpsPlayer>(GetOwner());
	if (!pPlayer)
	{
		UE_LOG(LogClass, Warning, TEXT("AAr::CtoS_StopShoot_Implementation / !pPlayer"));
		return;
	}
	pPlayer->bIsFire = false;
}


void AAr::CtoS_SetCurrentShootMode_Implementation(EShootMode NewShootMode)
{
	CurShootMode = NewShootMode;
}

void AAr::UpdateBulletCnt()
{
	//필요한 총알 수 채워넣기
	int iAddBulletCnt = 30 - iCurBulletCnt;

	//필요한 총알 수를 가지고 있는 경우
	if (iAllBulletCnt >= iAddBulletCnt)
	{
		iCurBulletCnt = 30;
		iAllBulletCnt -= iAddBulletCnt;
	}
	//가지고 있지 않은 경우
	else
	{
		iCurBulletCnt += iAllBulletCnt;
		iAllBulletCnt = 0;
	}
}

void AAr::OnRep_BulletCnt()
{
	//UI Update
	AFpsPlayer * pPlayer = Cast<AFpsPlayer>(GetOwner());
	if (!pPlayer)
	{
		UE_LOG(LogClass, Warning, TEXT("AAr::OnRep_iAllBulletCnt / !pPlayer"));
		return;
	}

	AGamePlayPC * pPc = Cast<AGamePlayPC>(pPlayer->GetController());
	if (!pPc)
	{
		UE_LOG(LogClass, Warning, TEXT("AAr::OnRep_iAllBulletCnt / !pPc"));
		return;
	}
	pPc->UpdateRemainBulletUI(iCurBulletCnt, iAllBulletCnt);
}

void AAr::UpdateCurrentReboundRate(bool bInit)
{
	if (bInit)
	{
		CurrentReboundRate = InitReboundRate;
		return;
	}

	CurrentReboundRate += IncreaseReboundRate;
}

void AAr::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAr, iAllBulletCnt);
	DOREPLIFETIME(AAr, iCurBulletCnt);
	DOREPLIFETIME(AAr, CurShootMode);
}

void AAr::StoA_SpawnMuzzleFlashAndFireSound_Implementation()
{
	if (MuzzleFlash && FireSound)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleFlash,
			WeaponBody,
			TEXT("b_Muzzle"),
			FVector(0.0f),
			FRotator(90.0f,0.0f,0.0f),
			FVector(0.4f));
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(),
			FireSound, WeaponBody->GetSocketLocation(TEXT("b_Muzzle")));
	}
}

void AAr::StoA_SpawnNoBulletSound_Implementation()
{
	if (NoBulletSound)
	{
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(),
			NoBulletSound, WeaponBody->GetSocketLocation(TEXT("b_Muzzle")));
	}
}

// Sets default values
AAr::AAr()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponBody = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Body"));

	Sight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sight"));
	Sight->SetupAttachment(WeaponBody,TEXT("RailSocket"));

	MuzzleFlash = CreateDefaultSubobject<UParticleSystem>(TEXT("MuzzleFlash"));

	FireSound = CreateDefaultSubobject<USoundBase>(TEXT("FireSound"));

	CurrentReboundRate = InitReboundRate;
}

// Called when the game starts or when spawned
void AAr::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AAr::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

