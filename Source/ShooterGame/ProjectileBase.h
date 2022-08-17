// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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

protected:
	UFUNCTION()
	virtual void OnProjectileHit(UPrimitiveComponent *HitComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit);

public:
	virtual void Destroyed() override;
};
