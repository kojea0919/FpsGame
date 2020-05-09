// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArBullet.generated.h"

UCLASS()
class FPS_API AArBullet : public AActor
{
	GENERATED_BODY()
	

public:
	//Components
	//-------------------------------------------------
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UBoxComponent * BoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UParticleSystemComponent * ParticleSystem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UProjectileMovementComponent * ProjectileMovement;
	//-------------------------------------------------
	//Components

public:	
	// Sets default values for this actor's properties
	AArBullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void DoActorBeginOverlap(class UPrimitiveComponent * OverlappedComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	float fBulletDamage = 34.0f;

	void ApplyDamage(const FHitResult & SweepResult);

	UFUNCTION(NetMulticast,Reliable)
	void StoA_DestoryBullet();
	void StoA_DestoryBullet_Implementation();


	bool bIsRedTeamBullet;
};
