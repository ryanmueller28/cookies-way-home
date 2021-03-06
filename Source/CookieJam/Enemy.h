// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UENUM(BlueprintType)
enum class EEnemyMovementStatus : uint8
{
	EMS_Idle UMETA(DisplayName = "Idle"),
	EMS_MoveToTarget UMETA(DisplayName = "MoveToTarget"),
	EMS_Searching UMETA(DisplayName = "Searching"),
	EMS_PickupCookie UMETA(DisplayName = "PickupCookie"),

	EMS_MAX UMETA(DisplayName = "DefaultMAX")
};

UCLASS()
class COOKIEJAM_API AEnemy : public ACharacter
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemy();



	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	EEnemyMovementStatus EnemyMovementStatus;

	FORCEINLINE void SetEnemyMovementStatus(EEnemyMovementStatus Status) { EnemyMovementStatus = Status; }
	FORCEINLINE EEnemyMovementStatus GetEnemyMovementStatus() { return EnemyMovementStatus; }

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Senses")
	//class UPawnSensingComponent* Senses;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class AAIController* AIController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	class USphereComponent* PickupSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	USphereComponent* AggroSphere;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameOver")
	class UAnimMontage* PickupMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	class USoundBase* CaughtCookieSound;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void PickupSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	UFUNCTION()
	virtual void AggroSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void AggroSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	void MoveToTarget(class ACookieJamCharacter* Target);
};
