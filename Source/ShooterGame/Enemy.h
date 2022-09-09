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

	UPROPERTY(EditAnywhere, Category = "EnemProperties")
	UAnimMontage *AttackMontage;
	
	UPROPERTY(EditAnywhere, Category = "EnemProperties")
	UAnimMontage *DeathMontage;

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
	class USphereComponent *AttackSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EnemyProperties", meta = (AllowPrivateAccess = "true"))
	class AShooterCharacter *ShooterCharacter;

	UPROPERTY(VisibleAnywhere, Category = "EnemyProperties")
	float StunChance = 0.3f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "EnemyProperties", meta = (AllowPrivateAccess = "true"))
	bool bStunned = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "EnemyProperties", meta = (AllowPrivateAccess = "true"))
	bool bAttack = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "EnemyProperties", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent *HandBox1;

	UPROPERTY(EditAnywhere, Category = "EnemyProperties")
	float Damage = 5.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "EnemyProperties", meta = (AllowPrivateAccess = "true"))
	bool bDie = false;

protected:
	UFUNCTION()
	virtual void OnAgroBoxBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION()
	virtual void OnAgroBoxEndOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	virtual void OnAttackSphereBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION()
	virtual void OnAttackSphereEndOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	virtual void OnHandBox1BeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION()
	virtual void OnHandBox1EndOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex);

	void EnemyPatrol();

	void EnemyStun();

	UFUNCTION(BlueprintCallable, Category = "EnemyProperties")
	void PlayAttackMontage();

	void Death();

	void DestroyEnemy();

	virtual void Destroyed() override;

public:
	FORCEINLINE float GetHealth() const { return Health; }
	FORCEINLINE FString GetBone() const { return Bone; }

	FORCEINLINE void SetBone(FString Value) { Bone = Value; }

	FORCEINLINE UBehaviorTree *GetBehaviorTree() const { return BehaviorTree; }

	FORCEINLINE bool GetAttack() const { return bAttack; }
};
