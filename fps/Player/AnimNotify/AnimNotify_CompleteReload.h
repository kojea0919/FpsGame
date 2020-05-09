// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_CompleteReload.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API UAnimNotify_CompleteReload : public UAnimNotify
{
	GENERATED_BODY()

	//Reload¿Ï·á
	//------------------------------------------------------------------------------------
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);
	//------------------------------------------------------------------------------------
};
