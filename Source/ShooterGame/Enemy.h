// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UCLASS()
class SHOOTERGAME_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const &DamageEvent, AController *EventInstigator, AActor *DamageCauser) override;

	void ResetHitReactValue();

private:
	UPROPERTY(VisibleAnywhere, Category = "Enemy")
	float Health = 100.f;

	UPROPERTY(EditAnywhere, Category = "Enemy")
	float MaxHealth = 100.f;

	UPROPERTY(VisibleAnywhere, Category = "Enemy")
	FString Bone;

	UPROPERTY(EditAnywhere, Category = "EnemProperties")
	UAnimMontage *HitReactMontage;

	FTimerHandle HitReactTimer;

	bool bCanHitReact = true;

	UPROPERTY(EditAnywhere, Category = "EnemyProperties")
	class UBehaviorTree *BehaviorTree;

	UPROPERTY(EditAnywhere, Category = "EnemyProperties", meta = (MakeEditWidget = "true"));
	FVector PatrolPoint;

	UPROPERTY(EditAnywhere, Category = "EnemyProperties", meta = (MakeEditWidget = "true"));
	FVector PatrolPoint2;

	UPROPERTY(VisibleAnywhere, Category = "EnemyProperties")
	class AEnemyController *EnemyController;

	UPROPERTY(VisibleAnywhere, Category = "EnemyProperties")
	class UBoxComponent *AgroBox;

	UPROPERTY(VisibleAnywhere, Category = "EnemyProperties")
	class AShooterCharacter *ShooterCharacter;

protected:
	UFUNCTION()
	virtual void OnAgroBoxBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION()
	virtual void OnAgroBoxEndOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex);

	void EnemyPatrol();

public:
	FORCEINLINE float GetHealth() const { return Health; }
	FORCEINLINE FString GetBone() const { return Bone; }

	FORCEINLINE void SetBone(FString Value) { Bone = Value; }

	FORCEINLINE UBehaviorTree *GetBehaviorTree() const { return BehaviorTree; }
};
