// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ar.generated.h"


UENUM(BlueprintType)
enum class EShootMode : uint8
{
	e_SingleShot = 0		UMETA(DisplayName = "SingleShot"),
	e_Repeater = 1			UMETA(DisplayName = "Repeater")
};

UCLASS()
class FPS_API AAr : public AActor
{
	GENERATED_BODY()
	
public:
	//Component
	//----------------------------------------------
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	class USkeletalMeshComponent * WeaponBody;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent * Sight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UParticleSystem * MuzzleFlash;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USoundBase * FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USoundBase * NoBulletSound;
	//----------------------------------------------
	//Component


	//�߻�
	//----------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bullet")
	TSubclassOf<class AArBullet> ArBullet_Template;
	
	UFUNCTION(Server,Reliable)
	void CtoS_Shoot();
	void CtoS_Shoot_Implementation();

	//������ �Ѿ� ������û
	void SpawnBullet();


	UFUNCTION(Server, Reliable)
	void CtoS_StopShoot();
	void CtoS_StopShoot_Implementation();

	//�߻���
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Replicated)
	EShootMode CurShootMode = EShootMode::e_SingleShot;

	EShootMode GetCurrentShootMode() { return CurShootMode; }

	UFUNCTION(Server, Reliable)
	void CtoS_SetCurrentShootMode(EShootMode NewShootMode); 
	void CtoS_SetCurrentShootMode_Implementation(EShootMode NewShootMode);
	//----------------------------------------------

	//Reload
	//----------------------------------------------
	//AnimNotify_CompleteReload���� ȣ��
	void UpdateBulletCnt();
	//----------------------------------------------

	//Bullet Cnt
	//----------------------------------------------
	//���� źâ ������ ���� �Ѿ� ��
	UPROPERTY(BlueprintReadOnly,VisibleAnywhere,ReplicatedUsing="OnRep_BulletCnt")
	int iAllBulletCnt = 150;

	//���� źâ �Ѿ� ��
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, ReplicatedUsing = "OnRep_BulletCnt")
	int iCurBulletCnt = 30;

	UFUNCTION()
	void OnRep_BulletCnt();

	int GetAllBulletCnt() { return iAllBulletCnt; }

	int GetCurBulletCnt() { return iCurBulletCnt; }
	//----------------------------------------------

	//�ѱ� �ݵ�
	//----------------------------------------------
	float GetCurrentReboundRate() { return CurrentReboundRate; }

	//bIsInit == ture�� CurrentReboundRate�� �ʱ���·� 
	//false�� CurrentReboundRate����
	void UpdateCurrentReboundRate(bool bInit);
	//----------------------------------------------

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	//Muzzle Particle And Sound Effect
	//----------------------------------------------
	UFUNCTION(NetMulticast, Reliable)
	void StoA_SpawnMuzzleFlashAndFireSound();
	void StoA_SpawnMuzzleFlashAndFireSound_Implementation();
	//----------------------------------------------

	//�Ѿ��� ���� �� �߻�� Sound Effect
	//----------------------------------------------
	UFUNCTION(NetMulticast,Reliable)
	void StoA_SpawnNoBulletSound();
	void StoA_SpawnNoBulletSound_Implementation();
	//----------------------------------------------

	//Rebound
	//----------------------------------------------
	UPROPERTY(EditAnywhere)
	float InitReboundRate = -0.1f;

	UPROPERTY(EditAnywhere)
	float IncreaseReboundRate = -0.1f;

	float CurrentReboundRate;
	//----------------------------------------------


	//���� TimerHandle
	FTimerHandle RepeaterTimerHandle;
	//----------------------------------------------

public:	
	// Sets default values for this actor's properties
	AAr();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

