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



	//�� ����
	//----------------------------------------------------
	bool IsRedTeam();

	UFUNCTION(NetMulticast,Reliable)
	void StoA_UpdateTeamWidget(bool bIsRedTeam);
	void StoA_UpdateTeamWidget_Implementation(bool bIsRedTeam);

	//���� �÷��̾��� ���� ���� WidgetComponent Visibility ����
	void ShowTeamWidget(bool bIsRedTeam);
	//----------------------------------------------------
	//�� ����



	//�Է¿� ���� ��������Ʈ �Լ�
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
	//�Է¿� ���� ��������Ʈ �Լ�


	//Player ����
	//----------------------------------------------------
	
	//Player ���� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated)
	EPlayerState CurPlayerState = EPlayerState::e_Normal;

	//�÷��̾� ���� Set�Լ�
	UFUNCTION(Server,Reliable)
	void CtoS_SetPlayerState(EPlayerState NewState);
	void CtoS_SetPlayerState_Implementation(EPlayerState NewState);

	//������ �� �̵��ӵ�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float WalkSpeed = 600.0f;

	//������ �� �̵��ӵ�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float AimSightWalkSpeed = 300.0f;

	//�ɾ����� �� �̵��ӵ�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float CrouchSpeed = 150.0f;

	//Reloading�� �̵��ӵ�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float ReloadingSpeed = 450.0f;

	//���� ���¿� ���� �̵��ӵ� Update
	void UpdateMoveSpeed();

	//Aim�� Pitch��
	float GetAimPitch();

	bool IsDead() { return CurPlayerState == EPlayerState::e_Dead ? true : false; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	bool bIsFire = false;
	
	UFUNCTION(Server, Reliable)
	void CtoS_SetPlayerIsFire(bool bIsPlayerFire);
	void CtoS_SetPlayerIsFire_Implementation(bool bIsPlayerFire);
	//----------------------------------------------------
	//Player ����


	//����
	//----------------------------------------------------
	//Ar�� ����� Blueprint
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class AAr> AssertRifle_Template;

	UPROPERTY(BlueprintReadOnly,VisibleAnywhere,Replicated)
	class AAr * pAr;

	//����
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UAnimMontage * ReloadMontage;


	//bLeft�� true�� �޼տ� attach
	//false�� �����տ� attach
	void SetWeaponAttachToHandSight(bool bLeft);

	//���� �ݵ� ����
	UFUNCTION(Client,Reliable)
	void StoC_ApplyWeaponRebound(float ReboundRate);
	void StoC_ApplyWeaponRebound_Implementation(float ReboundRate);
	//----------------------------------------------------
	//����




	//������
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
	//������


	//������
	//-----------------------------------------------------
	void SetRespawnPlayerState();

	UFUNCTION(Client, Reliable)
	void StoC_UpdateBulletCntUI(int iCurBulletCnt, int iAllBulletCnt);
	void StoC_UpdateBulletCntUI_Implementation(int iCurBulletCnt, int iAllBulletCnt);
	//-----------------------------------------------------
	//������


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	AFpsPlayer();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	//Blood Particle���� �Լ�
	UFUNCTION(NetMulticast,Reliable)
	void StoA_SpawnBloodEffect(const FHitResult & HitResult);
	void StoA_SpawnBloodEffect_Implementation(const FHitResult & HitResult);

	//�ִϸ��̼� ���
	//------------------------------------------------
	//���ε� �ִϸ��̼� ���
	UFUNCTION(Server, Reliable)
	void CtoS_PlayReloadAnimation();
	void CtoS_PlayReloadAnimation_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void StoA_PlayReloadAnimation();
	void StoA_PlayReloadAnimation_Implementation();


	//�׾��� �� �ִϸ��̼� ���
	UFUNCTION(NetMulticast, Reliable)
	void StoA_PlayDeadAnimation();
	void StoA_PlayDeadAnimation_Implementation();

	//dead�ִϸ��̼� ��� ����
	UFUNCTION(NetMulticast, Reliable)
	void StoA_StopDeadAnimation();
	void StoA_StopDeadAnimation_Implementation();

	//�������� �Ծ��� �� �ִϸ��̼� ���
	UFUNCTION(NetMulticast, Reliable)
	void StoA_PlayHitAnimation();
	void StoA_PlayHitAnimation_Implementation();
	//------------------------------------------------
};

