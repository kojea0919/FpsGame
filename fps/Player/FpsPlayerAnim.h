// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "FpsPlayerAnim.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API UFpsPlayerAnim : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	//ĳ���� �̵��ӵ�
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	float Speed;

	//ĳ���� �̵�����
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MoveDirection;

	//���� ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AimDirection;

	//�ɾ��ִ� ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsCrouched;

	//����
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsJumping;

	//Fire Animation Alpha
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float FireAlpha;
};
