// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponTypes.h"
#include "ProjectileBase.generated.h"

UCLASS()
class SHOOTERGAME_API AProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, Category = "ProjectileBase")
	class UBoxComponent *ProjectileBox;
	
	UPROPERTY(VisibleAnywhere, Category = "ProjectileBase")
	class UStaticMeshComponent *ProjectileMesh;

	UPROPERTY(VisibleAnywhere, Category = "ProjectileBase")
	class UProjectileMovementComponent *ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, Category = "ProjectileBase")
	class UParticleSystem *BulletImpactParticles;

	UPROPERTY(EditAnywhere, Category = "ProjectileBase")
	class UParticleSystemComponent *BulletTracer;

	UPROPERTY(EditAnywhere, Category = "ProjectileBase")
	class USoundCue *HitSound;

	UPROPERTY(EditAnywhere, Category = "ProjectileBase")
	EWeaponType WeaponType;

	FTimerHandle GrenadeDestroyTimer;

	UPROPERTY(EditAnywhere, Category = "ProjectileBase")
	float GrenadeDestroyTime = 5.f;

protected:
	UFUNCTION()
	virtual void OnProjectileHit(UPrimitiveComponent *HitComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit);

	void DestroyProjectile();

public:
	virtual void Destroyed() override;
};
