// Fill out your copyright notice in the Description page of Project Settings.


#include "FpsPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/InputComponent.h"
#include "Components/WidgetComponent.h"
#include "TimerManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"
#include "Weapon/Ar.h"
#include "Game/GamePlay/GamePlayPC.h"
#include "Game/GamePlay/GamePlayGM.h"
#include "Game/GamePlay/GamePlayGS.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "UnrealNetwork.h"
#include "Engine.h" 

#include "GameInstance/GameInfoInstance.h"

void AFpsPlayer::ShowTeamWidget(bool bIsRedTeam)
{
	if (bIsRedTeam)
	{
		RedTeamWidget->SetVisibility(true);
	}
	else
	{
		BlueTeamWidget->SetVisibility(true);
	}
}

bool AFpsPlayer::IsRedTeam()
{
	AGamePlayPC * pPc = Cast<AGamePlayPC>(GetController());
	
	return pPc->IsRedTeam();
}

void AFpsPlayer::StoA_UpdateTeamWidget_Implementation(bool bIsRedTeam)
{
	AGamePlayPC * pLocalPC = Cast<AGamePlayPC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!pLocalPC)
	{
		UE_LOG(LogClass, Warning, TEXT("AFpsPlayer::UpdateTeamWidget / !pLocalPC"));
		return;
	}

	if (bIsRedTeam == pLocalPC->IsRedTeam())
	{
		ShowTeamWidget(bIsRedTeam);
	}
}

void AFpsPlayer::MoveForward(float Value)
{
	if (Value != 0 && CurPlayerState != EPlayerState::e_Dead)
	{
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AFpsPlayer::MoveRight(float Value)
{
	if (Value != 0 && CurPlayerState != EPlayerState::e_Dead)
	{
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AFpsPlayer::PlayerJump()
{
	if(CurPlayerState != EPlayerState::e_Dead)
		Jump();
}

void AFpsPlayer::PlayerJumpStop()
{
	StopJumping();
}

void AFpsPlayer::Turn(float Value)
{
	if (Value != 0 && CurPlayerState != EPlayerState::e_Dead)
	{
		AddControllerYawInput(Value);
	}
}

void AFpsPlayer::LookUp(float Value)
{
	if (Value != 0 && CurPlayerState != EPlayerState::e_Dead)
	{
		AddControllerPitchInput(Value);
	}
}

void AFpsPlayer::DoCrouch()
{
	CanCrouch() ? Crouch() : UnCrouch();
}

void AFpsPlayer::CameraActivateForState()
{
	if (CurPlayerState == EPlayerState::e_Dead)
		return;

	if (CurPlayerState == EPlayerState::e_Normal)
	{
		//플레이어 상태 Change
		CurPlayerState = EPlayerState::e_AimSight;
		UpdateMoveSpeed();

		CtoS_SetPlayerState(EPlayerState::e_AimSight);

		//카메라 Change
		Camera->Deactivate();
		AimSightCamera->Activate();
	}
	else
	{
		CurPlayerState = EPlayerState::e_Normal;
		UpdateMoveSpeed();

		CtoS_SetPlayerState(EPlayerState::e_Normal);

		Camera->Activate();
		AimSightCamera->Deactivate();
	}
}

void AFpsPlayer::Fire()
{
	if (CurPlayerState == EPlayerState::e_Dead || CurPlayerState == EPlayerState::e_Reloading)
		return;

	//Shoot
	pAr->CtoS_Shoot();
}

void AFpsPlayer::StopFire()
{
	pAr->CtoS_StopShoot();
}

void AFpsPlayer::Reload()
{
	//Reload가 불가능한 경우
	if (pAr->GetCurBulletCnt() == 30 || pAr->GetAllBulletCnt() == 0)
		return;

	//일반 카메라 활성화
	if (CurPlayerState == EPlayerState::e_AimSight)
	{
		AimSightCamera->Deactivate();
		Camera->Activate();
	}

	CtoS_PlayReloadAnimation();

	CurPlayerState = EPlayerState::e_Reloading;
	UpdateMoveSpeed();

	CtoS_SetPlayerState(EPlayerState::e_Reloading);
}

void AFpsPlayer::FireModeChange()
{
	AGamePlayPC * pPc = Cast<AGamePlayPC>(GetController());
	if (nullptr == pPc)
	{
		UE_LOG(LogClass, Warning, TEXT("AFpsPlayer::FireModeChange / pPc nullptr"));
	}

	if (pAr->GetCurrentShootMode() == EShootMode::e_SingleShot)
	{
		pAr->CurShootMode = EShootMode::e_Repeater;
		pAr->CtoS_SetCurrentShootMode(EShootMode::e_Repeater);
		pPc->ShowRepeatUI();
	}
	else
	{
		pAr->CurShootMode = EShootMode::e_SingleShot;
		pAr->CtoS_SetCurrentShootMode(EShootMode::e_SingleShot);
		pPc->ShowSingleUI();
	}
}


void AFpsPlayer::CtoS_PlayReloadAnimation_Implementation()
{
	StoA_PlayReloadAnimation();
}

void AFpsPlayer::StoA_PlayReloadAnimation_Implementation()
{
	SetWeaponAttachToHandSight(false);
	PlayAnimMontage(ReloadMontage);
}

void AFpsPlayer::SetWeaponAttachToHandSight(bool bLeft)
{
	if(bLeft)
		pAr->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), TEXT("WeaponPosition_l"));
	else
		pAr->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), TEXT("WeaponPosition_r"));
}

void AFpsPlayer::StoC_ApplyWeaponRebound_Implementation(float ReboundRate)
{
	AddControllerPitchInput(ReboundRate);
}

void AFpsPlayer::OnRep_CurrentHP()
{
	AGamePlayPC * pPc = Cast<AGamePlayPC>(GetController());
	if (nullptr == pPc)
	{
		UE_LOG(LogClass, Warning, TEXT("AFpsPlayer::OnRep_CurrentHP / pPc nullptr"));
		return;
	}

	if(pPc->PlayerMainWidget)
		pPc->SetHPBar(CurrentHP / MaxHP);
}

float AFpsPlayer::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	FPointDamageEvent* PointDamageEvent = (FPointDamageEvent*)&DamageEvent;

	//Blood Effect
	//-----------------------------------------------------------------------------
	StoA_SpawnBloodEffect(PointDamageEvent->HitInfo);
	//-----------------------------------------------------------------------------

	//죽은 경우 pass
	//-----------------------------------------------------------------------------
	if (EPlayerState::e_Dead == CurPlayerState)
		return 0.0f;
	//-----------------------------------------------------------------------------


	//Hit Animation 
	//-----------------------------------------------------------------------------
	StoA_PlayHitAnimation();
	//-----------------------------------------------------------------------------


	//HPBar 갱신
	//-----------------------------------------------------------------------------
	CurrentHP -= DamageAmount;

	OnRep_CurrentHP();
	//-----------------------------------------------------------------------------

	//Dead
	//-----------------------------------------------------------------------------
	if (CurrentHP <= 0)
	{
		StoA_PlayDeadAnimation();

		CurPlayerState = EPlayerState::e_Dead;

		AGamePlayPC * pDeathPlayerPc = GetController<AGamePlayPC>();
		AGamePlayPC * pKillPlayerPc = Cast<AGamePlayPC>(EventInstigator);
		AGamePlayGM * pGm = Cast<AGamePlayGM>(UGameplayStatics::GetGameMode(GetWorld()));
		if(pGm && pDeathPlayerPc && pKillPlayerPc)
		{
			//KillScore UI Update
			pGm->UpdateKillScore(IsRedTeam());
			
			//Respawn Procedure
			pGm->PlayerDead(pKillPlayerPc->iPlayerNumberID, pDeathPlayerPc->iPlayerNumberID);
		}
	}
	//-----------------------------------------------------------------------------
	//Dead

	return 0.0f;
}

void AFpsPlayer::StoA_PlayHitAnimation_Implementation()
{
	PlayAnimMontage(HitMontage);
}


void AFpsPlayer::StoA_PlayDeadAnimation_Implementation()
{
	PlayAnimMontage(DeadMontage);

	//Physics Simulate On
	FDetachmentTransformRules DetachRule(EDetachmentRule::KeepWorld, false);
	pAr->WeaponBody->SetCollisionProfileName(TEXT("Weapon"));
	pAr->WeaponBody->SetSimulatePhysics(true);
	pAr->DetachFromActor(DetachRule);


	AGamePlayPC * pPc = Cast<AGamePlayPC>(GetController());
	if (nullptr == pPc)
	{
		UE_LOG(LogClass, Warning, TEXT("AFpsPlayer::PlayDeadAnimation() / pPc nullptr"));
		return;
	}

	//Player 스폰대기 처리
	//------------------------------
	if (pPc->IsLocalController())
	{
		DisableInput(pPc);
	}
	//------------------------------
}

void AFpsPlayer::SetRespawnPlayerState()
{
	//플레이어 State Update
	CurPlayerState = EPlayerState::e_Normal;

	//체력 초기화
	CurrentHP = MaxHP;
	OnRep_CurrentHP();

	//재생중이던 DeadAnimation 종료
	StoA_StopDeadAnimation();

	//총 생성
	pAr->Destroy();
	AAr * pAAr = GetWorld()->SpawnActor<AAr>(AssertRifle_Template);
	if (nullptr == pAAr)
	{
		UE_LOG(LogClass, Warning, TEXT("AFpasPlayer::SetRespawnPlayerState / pAAr nullptr"));
		return;
	}
	pAAr->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), TEXT("WeaponPosition_l"));

	StoC_UpdateBulletCntUI(pAAr->GetCurBulletCnt(), pAAr->GetAllBulletCnt());

	pAr = pAAr;
	pAr->SetOwner(this);
}

void AFpsPlayer::StoA_StopDeadAnimation_Implementation()
{
	StopAnimMontage(DeadMontage);

	AGamePlayPC * pPc = Cast<AGamePlayPC>(GetController());
	if (nullptr == pPc)
	{
		UE_LOG(LogClass, Warning, TEXT("AFpsPlayer::StopDeadAnimation() / pPc nullptr"));
		return;
	}

	if (pPc->IsLocalController())
	{
		EnableInput(pPc);
	}
}

void AFpsPlayer::StoC_UpdateBulletCntUI_Implementation(int iCurBulletCnt, int iAllBulletCnt)
{
	AGamePlayPC * pPc = Cast<AGamePlayPC>(GetController());
	if (nullptr == pPc)
	{
		UE_LOG(LogClass, Warning, TEXT("AFpsPlayer::UpdateBulletCntUI_Implementation() / pPc nullptr"));
		return;
	}

	pPc->UpdateRemainBulletUI(iCurBulletCnt, iAllBulletCnt);
}


void AFpsPlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFpsPlayer, CurPlayerState);
	DOREPLIFETIME(AFpsPlayer, pAr);
	DOREPLIFETIME(AFpsPlayer, CurrentHP);
	DOREPLIFETIME(AFpsPlayer, bIsFire);
}

// Sets default values
AFpsPlayer::AFpsPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -88.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetCollisionProfileName(TEXT("Player"));

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(GetMesh(), TEXT("Fps_Position"));
	Camera->SetRelativeScale3D(FVector(0.2f));
	Camera->SetRelativeLocation(FVector(2.0f, 0.0f, 4.7f));
	Camera->bUsePawnControlRotation = true;
	

	AimSightCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("AimSightCamera"));
	AimSightCamera->SetupAttachment(GetMesh(), TEXT("AimSight"));
	AimSightCamera->SetRelativeScale3D(FVector(0.2f));
	AimSightCamera->bUsePawnControlRotation = true;

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	GetCharacterMovement()->MaxWalkSpeedCrouched = CrouchSpeed;
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;

	RedTeamWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("RedTeamWidget"));
	RedTeamWidget->SetCollisionProfileName(TEXT("NoCollision"));
	RedTeamWidget->SetVisibility(true);
	RedTeamWidget->SetupAttachment(GetMesh());

	BlueTeamWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("BlueTeamWidget"));
	BlueTeamWidget->SetCollisionProfileName(TEXT("NoCollision"));
	BlueTeamWidget->SetVisibility(true);
	BlueTeamWidget->SetupAttachment(GetMesh());
}

// Called when the game starts or when spawned
void AFpsPlayer::BeginPlay()
{
	Super::BeginPlay();

	Camera->Activate();
	AimSightCamera->Deactivate();	

	if (Role < ROLE_Authority)
	{
		return;
	}

	//무기 생성
	//--------------------------------------------------------------------
	AAr * pAAr = GetWorld()->SpawnActor<AAr>(AssertRifle_Template);
	if (nullptr == pAAr)
	{
		UE_LOG(LogClass, Warning, TEXT("AFpasPlayer::BeginPlay / pAAr nullptr"));
		return;
	}
	pAAr->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), TEXT("WeaponPosition_l"));
	
	pAr = pAAr;
	pAr->SetOwner(this);
	//--------------------------------------------------------------------


}

// Called every frame
void AFpsPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AFpsPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AFpsPlayer::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AFpsPlayer::MoveRight);

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AFpsPlayer::PlayerJump);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Released, this, &AFpsPlayer::PlayerJumpStop);

	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AFpsPlayer::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AFpsPlayer::LookUp);

	PlayerInputComponent->BindAction(TEXT("Crouch"), IE_Pressed, this, &AFpsPlayer::DoCrouch);

	PlayerInputComponent->BindAction(TEXT("Aim"), IE_Pressed, this, &AFpsPlayer::CameraActivateForState);

	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &AFpsPlayer::Fire);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Released, this, &AFpsPlayer::StopFire);

	PlayerInputComponent->BindAction(TEXT("Reload"), IE_Pressed, this, &AFpsPlayer::Reload);

	PlayerInputComponent->BindAction(TEXT("FireModeChange"), IE_Pressed, this, &AFpsPlayer::FireModeChange);
}

void AFpsPlayer::StoA_SpawnBloodEffect_Implementation(const FHitResult & HitResult)
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
		BloodEffect,
		HitResult.ImpactPoint + (HitResult.ImpactNormal * 3.0f));
}


void AFpsPlayer::UpdateMoveSpeed()
{
	switch (CurPlayerState)
	{
	case EPlayerState::e_Normal:
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		break;
	}
	case EPlayerState::e_AimSight:
	{
		GetCharacterMovement()->MaxWalkSpeed = AimSightWalkSpeed;
		break;
	}
	case EPlayerState::e_Reloading:
	{
		GetCharacterMovement()->MaxWalkSpeed = ReloadingSpeed;
		break;
	}
	default:
	{
		UE_LOG(LogClass, Warning, TEXT("AFpsPlayer::UpdateMoveSpeed / CurPlayerState Invalid Value"))
			break;
	}
	}
}

float AFpsPlayer::GetAimPitch()
{
	FVector AimVector = GetBaseAimRotation().Vector();
	
	float AimPitch = ActorToWorld().InverseTransformVectorNoScale(AimVector).Rotation().Pitch;

	return AimPitch;
}

void AFpsPlayer::CtoS_SetPlayerIsFire_Implementation(bool bIsPlayerFire)
{
	bIsFire = bIsPlayerFire;
}

void AFpsPlayer::CtoS_SetPlayerState_Implementation(EPlayerState NewState)
{
	CurPlayerState = NewState;
	UpdateMoveSpeed();
}
