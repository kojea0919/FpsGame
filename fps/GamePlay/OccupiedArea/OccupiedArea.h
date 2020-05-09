// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Player/FpsPlayer.h"
#include "OccupiedArea.generated.h"



//점령 지역 점령 상태
UENUM(BlueprintType)
enum class EOccupiedState : uint8
{
	e_NotOccupied = 0	UMETA(DisplayName = "NotOccupied"),
	e_RedTeamOccupied = 1	UMETA(DisplayName = "RedTeamOccupied"),
	e_BlueTeamOccupied = 2		UMETA(DisplayName = "BlueTeamOccupied")
};

UCLASS()
class FPS_API AOccupiedArea : public AActor
{
	GENERATED_BODY()
	

public:
	//Components
	//--------------------------------------------------------
	UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
	class UBoxComponent * OcuupiedAreaBox;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UWidgetComponent * OccupiedAreaNameWidget;
	//--------------------------------------------------------
	//Components

	void SetOccupiedAreaBoxNoCollision();


	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FText OccupiedAreaName;


	UFUNCTION()
	void DoActorBeginOverlap(class UPrimitiveComponent * OverlappedComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void DoActorEndOverlap(class UPrimitiveComponent * OevrlapeedComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);


	//점령 지역 안에있는 플레이어
	TArray<class AFpsPlayer*> OverlapedPlayer;

	//현재 점령 상태
	//---------------------------------------------------------------------------------------
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, ReplicatedUsing = "OnRep_CurOccupiedState")
	EOccupiedState CurOccupiedState = EOccupiedState::e_NotOccupied;

	UFUNCTION()
	void OnRep_CurOccupiedState();
	//---------------------------------------------------------------------------------------


	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Replicated)
	int iRedTeamCnt = 0;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Replicated)
	int iBlueTeamCnt = 0;


	//Client에 복사될 점령 점수
	//---------------------------------------------------------------------------------------
	//점령 상태가 변할 때 마다 Server에서 값을 복사
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, ReplicatedUsing = "OnRep_ReplicateOccupiedScore")
	float ReplicateOccupiedScore;

	UFUNCTION()
	void OnRep_ReplicateOccupiedScore();
	//---------------------------------------------------------------------------------------


public:	
	// Sets default values for this actor's properties
	AOccupiedArea();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void Client_ComputeOccupiedScore(float AddScore, EOccupiedState OcuupiedStateOfThisLoop);
	void Server_ComputeOccupiedScore(float AddScore, EOccupiedState OcuupiedStateOfThisLoop);

	
	float ComputeAddScore(float DeltaTime, EOccupiedState & OcuupiedStateOfThisLoop);

	void UpdateProgressBar(float DeltaTime);

	void CheckOccupiedState(float DeltaTime);


	//점령 점수
	//-------------------------------------------
	float fOccupiedScore = 0.0f;
	float fMaxScore = 100.0f;

	float fOccupiedSpeed = 25.0f;
	//-------------------------------------------


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
