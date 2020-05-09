// Fill out your copyright notice in the Description page of Project Settings.


#include "ArBullet.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/FpsPlayer.h"
#include "Weapon/Ar.h"
#include "Engine.h"




// Sets default values
AArBullet::AArBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxComponent->SetupAttachment(ParticleSystem);
	BoxComponent->SetRelativeScale3D(FVector(0.2f, 0.05f, 0.05f));
	BoxComponent->SetCollisionProfileName(TEXT("Bullet"));
	RootComponent = BoxComponent;
	if (UKismetSystemLibrary::IsServer(GetWorld()))
	{
		BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AArBullet::DoActorBeginOverlap);
	}


	ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	ParticleSystem->SetRelativeScale3D(FVector(0.4f));
	ParticleSystem->SetupAttachment(BoxComponent);


	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	ProjectileMovement->MaxSpeed = 2500.0f;
	ProjectileMovement->InitialSpeed = 2500.0f;
	
}

// Called when the game starts or when spawned
void AArBullet::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AArBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AArBullet::DoActorBeginOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{	
	AFpsPlayer * pPlayer = Cast<AFpsPlayer>(OtherActor);
	if (pPlayer)
	{
		if (pPlayer->IsRedTeam() != bIsRedTeamBullet)
		{
			ApplyDamage(SweepResult);
		}
	}

	StoA_DestoryBullet();
}


void AArBullet::ApplyDamage(const FHitResult & SweepResult)
{
	FName HitBoneName = SweepResult.BoneName;
	if (HitBoneName.Compare(TEXT("head"))==0)
	{
		fBulletDamage = 100.0f;
	}
	else if (HitBoneName.Compare(TEXT("foot_l")) == 0|| HitBoneName.Compare(TEXT("foot_r")) == 0||
		HitBoneName.Compare(TEXT("hand_l")) == 0|| HitBoneName.Compare(TEXT("hand_r")) == 0)
	{
		fBulletDamage = 10.0f;
	}
	else
	{
		fBulletDamage = 34.0f;
	}

	AFpsPlayer * pPlayer = Cast<AFpsPlayer>(GetOwner());
	if (!pPlayer)
	{
		UE_LOG(LogClass, Warning, TEXT("AArBullet::ApplyDamage / !pPlayer"));
		return;
	}

	AController * pPc = pPlayer->GetController();
	if (!pPc)
	{
		UE_LOG(LogClass, Warning, TEXT("AArBullet::ApplyDamage / !pPc"));
		return;
	}


	UGameplayStatics::ApplyPointDamage(SweepResult.Actor.Get(),
		fBulletDamage,
		-SweepResult.ImpactNormal,
		SweepResult,
		pPc,
		this,
		NULL);
}

void AArBullet::StoA_DestoryBullet_Implementation()
{
	Destroy();
}

