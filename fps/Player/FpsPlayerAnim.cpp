// Fill out your copyright notice in the Description page of Project Settings.


#include "FpsPlayerAnim.h"
#include "FpsPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/CameraComponent.h"
#include "Engine.h"

void UFpsPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	AFpsPlayer * pPlayer = Cast<AFpsPlayer>(TryGetPawnOwner());
	if(pPlayer)
	{
		//�̵��ӵ� Setting
		FVector PlayerVelocity = pPlayer->GetCharacterMovement()->Velocity;
		Speed = PlayerVelocity.Size();
		//-------------------------------------------

		//�����̴� ���� Setting
		//���� -90, ������ 90
		FRotator PlayerRotator = pPlayer->GetActorRotation();
		float PlayerForwardYaw = PlayerRotator.Yaw;
		float fTemp;
		float Move; 
		UKismetMathLibrary::GetYawPitchFromVector(PlayerVelocity, Move, fTemp);
		MoveDirection = Move - PlayerForwardYaw;
		//-------------------------------------------

		//���� ���� Setting
		//�Ʒ� -90, �� 90
		AimDirection = pPlayer->GetAimPitch();

		//-------------------------------------------

		//�ɾ��ִ� ����
		bIsCrouched = pPlayer->bIsCrouched;

		//����
		bIsJumping = pPlayer->GetCharacterMovement()->IsFalling();

		//Fire Animation Additive Alpha
		//������ ��쿡�� ����
		FireAlpha = pPlayer->bIsFire ? 1.0f : 0.0f;
	}

}