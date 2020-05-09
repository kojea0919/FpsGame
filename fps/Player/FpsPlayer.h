// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FpsPlayer.generated.h"



UENUM(BlueprintType)
enum class EPlayerState : uint8
{
	e_Normal = 0	UMETA(DisplayName = "Normal"),
	e_AimSight = 1	UMETA(DisplayName = "AimSight"),
	e_Dead = 2		UMETA(DisplayName = "Dead"),
	e_Reloading = 3 UMETA(DisplayName = "Reloading")
};

UCLASS()
class FPS_API AFpsPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	//Component
	//----------------------------------------------------
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UCameraComponent * Camera;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UCameraComponent * AimSightCamera;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UWidgetComponent * RedTeamWidget;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UWidgetComponent * BlueTeamWidget;
	//----------------------------------------------------
	//Component



	//팀 관련
	//----------------------------------------------------
	bool IsRedTeam();

	UFUNCTION(NetMulticast,Reliable)
	void StoA_UpdateTeamWidget(bool bIsRedTeam);
	void StoA_UpdateTeamWidget_Implementation(bool bIsRedTeam);

	//현재 플레이어의 팀에 따라 WidgetComponent Visibility 설정
	void ShowTeamWidget(bool bIsRedTeam);
	//----------------------------------------------------
	//팀 관련



	//입력에 대한 델리게이트 함수
	//----------------------------------------------------
	void MoveForward(float Value);
	void MoveRight(float Value);

	void PlayerJump();
	void PlayerJumpStop();

	void Turn(float Value);
	void LookUp(float Value);

	void DoCrouch();
	void CameraActivateForState();
	void Fire();
	void StopFire();
	void Reload();
	void FireModeChange();
	//----------------------------------------------------
	//입력에 대한 델리게이트 함수


	//Player 상태
	//----------------------------------------------------
	
	//Player 현재 상태
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated)
	EPlayerState CurPlayerState = EPlayerState::e_Normal;

	//플레이어 상태 Set함수
	UFUNCTION(Server,Reliable)
	void CtoS_SetPlayerState(EPlayerState NewState);
	void CtoS_SetPlayerState_Implementation(EPlayerState NewState);

	//서있을 때 이동속도
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float WalkSpeed = 600.0f;

	//조준할 때 이동속도
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float AimSightWalkSpeed = 300.0f;

	//앉아있을 때 이동속도
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float CrouchSpeed = 150.0f;

	//Reloading중 이동속도
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float ReloadingSpeed = 450.0f;

	//현재 상태에 따른 이동속도 Update
	void UpdateMoveSpeed();

	//Aim의 Pitch값
	float GetAimPitch();

	bool IsDead() { return CurPlayerState == EPlayerState::e_Dead ? true : false; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	bool bIsFire = false;
	
	UFUNCTION(Server, Reliable)
	void CtoS_SetPlayerIsFire(bool bIsPlayerFire);
	void CtoS_SetPlayerIsFire_Implementation(bool bIsPlayerFire);
	//----------------------------------------------------
	//Player 상태


	//무기
	//----------------------------------------------------
	//Ar로 사용할 Blueprint
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class AAr> AssertRifle_Template;

	UPROPERTY(BlueprintReadOnly,VisibleAnywhere,Replicated)
	class AAr * pAr;

	//장전
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UAnimMontage * ReloadMontage;


	//bLeft가 true면 왼손에 attach
	//false면 오른손에 attach
	void SetWeaponAttachToHandSight(bool bLeft);

	//무기 반동 적용
	UFUNCTION(Client,Reliable)
	void StoC_ApplyWeaponRebound(float ReboundRate);
	void StoC_ApplyWeaponRebound_Implementation(float ReboundRate);
	//----------------------------------------------------
	//무기




	//데미지
	//-----------------------------------------------------
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float MaxHP = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,ReplicatedUsing = "OnRep_CurrentHP")
	float CurrentHP = 100.0f;

	UFUNCTION()
	void OnRep_CurrentHP();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UAnimMontage* HitMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UAnimMontage * DeadMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UParticleSystem * BloodEffect;
	//-----------------------------------------------------
	//데미지


	//리스폰
	//-----------------------------------------------------
	void SetRespawnPlayerState();

	UFUNCTION(Client, Reliable)
	void StoC_UpdateBulletCntUI(int iCurBulletCnt, int iAllBulletCnt);
	void StoC_UpdateBulletCntUI_Implementation(int iCurBulletCnt, int iAllBulletCnt);
	//-----------------------------------------------------
	//리스폰


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	AFpsPlayer();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	//Blood Particle생성 함수
	UFUNCTION(NetMulticast,Reliable)
	void StoA_SpawnBloodEffect(const FHitResult & HitResult);
	void StoA_SpawnBloodEffect_Implementation(const FHitResult & HitResult);

	//애니메이션 재생
	//------------------------------------------------
	//리로드 애니메이션 재생
	UFUNCTION(Server, Reliable)
	void CtoS_PlayReloadAnimation();
	void CtoS_PlayReloadAnimation_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void StoA_PlayReloadAnimation();
	void StoA_PlayReloadAnimation_Implementation();


	//죽었을 때 애니메이션 재생
	UFUNCTION(NetMulticast, Reliable)
	void StoA_PlayDeadAnimation();
	void StoA_PlayDeadAnimation_Implementation();

	//dead애니메이션 재생 정지
	UFUNCTION(NetMulticast, Reliable)
	void StoA_StopDeadAnimation();
	void StoA_StopDeadAnimation_Implementation();

	//데미지를 입었을 때 애니메이션 재생
	UFUNCTION(NetMulticast, Reliable)
	void StoA_PlayHitAnimation();
	void StoA_PlayHitAnimation_Implementation();
	//------------------------------------------------
};

