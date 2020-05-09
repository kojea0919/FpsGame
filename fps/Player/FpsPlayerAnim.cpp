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
		//이동속도 Setting
		FVector PlayerVelocity = pPlayer->GetCharacterMovement()->Velocity;
		Speed = PlayerVelocity.Size();
		//-------------------------------------------

		//움직이는 방향 Setting
		//왼쪽 -90, 오른쪽 90
		FRotator PlayerRotator = pPlayer->GetActorRotation();
		float PlayerForwardYaw = PlayerRotator.Yaw;
		float fTemp;
		float Move; 
		UKismetMathLibrary::GetYawPitchFromVector(PlayerVelocity, Move, fTemp);
		MoveDirection = Move - PlayerForwardYaw;
		//-------------------------------------------

		//에임 방향 Setting
		//아래 -90, 위 90
		AimDirection = pPlayer->GetAimPitch();

		//-------------------------------------------

		//앉아있는 상태
		bIsCrouched = pPlayer->bIsCrouched;

		//점프
		bIsJumping = pPlayer->GetCharacterMovement()->IsFalling();

		//Fire Animation Additive Alpha
		//연발인 경우에만 적용
		FireAlpha = pPlayer->bIsFire ? 1.0f : 0.0f;
	}

}