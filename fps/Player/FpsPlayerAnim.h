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

	//캐릭터 이동속도
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	float Speed;

	//캐릭터 이동방향
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MoveDirection;

	//에임 방향
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AimDirection;

	//앉아있는 상태
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsCrouched;

	//점프
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsJumping;

	//Fire Animation Alpha
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float FireAlpha;
};
