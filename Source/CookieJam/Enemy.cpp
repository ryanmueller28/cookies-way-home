// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

// Engine
#include "Perception/PawnSensingComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimInstance.h"
#include "AIController.h"
#include "Sound/SoundCue.h"
// Project
#include "CookieJamCharacter.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Senses = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Senses"));

	AggroSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AggroSphere"));
	AggroSphere->SetupAttachment(GetRootComponent());
	AggroSphere->InitSphereRadius(600.f);

	PickupSphere = CreateDefaultSubobject<USphereComponent>(TEXT("PickupSphere"));
	PickupSphere->SetupAttachment(GetRootComponent());
	PickupSphere->InitSphereRadius(75.f);
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	AIController = Cast<AAIController>(GetController());

	AggroSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::AggroSphereBeginOverlap);
	AggroSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::AggroSphereEndOverlap);
	PickupSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::PickupSphereBeginOverlap);
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

void AEnemy::PickupSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		ACookieJamCharacter* Character = Cast<ACookieJamCharacter>(OtherActor);

		if (Character)
		{
			if (CaughtCookieSound)
			{
				UGameplayStatics::PlaySound2D(this, CaughtCookieSound);
			}

			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

			if (AnimInstance)
			{
				AnimInstance->Montage_Play(PickupMontage, 1.0f);
				AnimInstance->Montage_JumpToSection(FName("PickupAnim"), PickupMontage);
				UGameplayStatics::OpenLevel(this, FName(GetWorld()->GetName()));
			}
		}
	}
}

void AEnemy::AggroSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		ACookieJamCharacter* Character = Cast<ACookieJamCharacter>(OtherActor);
		if (Character)
		{
			MoveToTarget(Character);
		}
	}
}

void AEnemy::AggroSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		ACookieJamCharacter* Character = Cast<ACookieJamCharacter>(OtherActor);

		if (Character)
		{
			SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Idle);
			if (AIController)
			{
				AIController->StopMovement();
			}
		}
	}
}

void AEnemy::MoveToTarget(ACookieJamCharacter* Target)
{
	SetEnemyMovementStatus(EEnemyMovementStatus::EMS_MoveToTarget);

	if (AIController)
	{
		FAIMoveRequest MoveRequest;

		MoveRequest.SetGoalActor(Target);
		MoveRequest.SetAcceptanceRadius(10.f);

		FNavPathSharedPtr NavPath;
		AIController->MoveTo(MoveRequest, &NavPath);
	}
}
