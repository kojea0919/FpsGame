// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GamePlayGS.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API AGamePlayGS : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly,VisibleAnywhere,ReplicatedUsing = "OnRep_RedTeamKillScore")
	int iRedTeamKillScore = 0;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, ReplicatedUsing = "OnRep_BlueTeamKillScore")
	int iBlueTeamKillScore = 0;

	UFUNCTION()
	void OnRep_RedTeamKillScore();
	UFUNCTION()
	void OnRep_BlueTeamKillScore();


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


};
