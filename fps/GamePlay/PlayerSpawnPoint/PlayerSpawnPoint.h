// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "PlayerSpawnPoint.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API APlayerSpawnPoint : public ATargetPoint
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool bIsRedTeamSpawnPoint;

	//플레이어가 Setting 됐는지 확인하기 위한 변수
	bool bIsSetPlayer;
};
