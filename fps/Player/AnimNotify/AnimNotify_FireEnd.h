// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_FireEnd.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API UAnimNotify_FireEnd : public UAnimNotify
{
	GENERATED_BODY()
	//FireAnimation 1 cycle end
	//------------------------------------------------------------------------------------
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);
	//------------------------------------------------------------------------------------
};
