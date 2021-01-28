// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelTransitionVolume.h"

// Engine
#include "Components/BoxComponent.h"
#include "Components/BillboardComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

// Project
#include "CookieJamCharacter.h"

// Sets default values
ALevelTransitionVolume::ALevelTransitionVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TransitionVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TransitionVolume"));
	RootComponent = TransitionVolume;

	Billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
	Billboard->SetupAttachment(GetRootComponent());

	TransitionLevelName = "DefaultLevel";
}

// Called when the game starts or when spawned
void ALevelTransitionVolume::BeginPlay()
{
	Super::BeginPlay();
	
	TransitionVolume->OnComponentBeginOverlap.AddDynamic(this, &ALevelTransitionVolume::OnOverlapBegin);
}

// Called every frame
void ALevelTransitionVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALevelTransitionVolume::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		ACookieJamCharacter* Character = Cast<ACookieJamCharacter>(OtherActor);

		if (Character)
		{
			if (EntrySound)
			{ 
				UGameplayStatics::PlaySound2D(this, EntrySound);
				Character->SwitchLevel(TransitionLevelName);
			}
			else 
			{
				Character->SwitchLevel(TransitionLevelName);
			}
		}
	}
}

