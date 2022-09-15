// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SupplyDrop.generated.h"

UCLASS()
class SHOOTERGAME_API ASupplyDrop : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASupplyDrop();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void DestroyActor();

private:
	UPROPERTY(VisibleAnywhere, Category = "SupplyDrop")
	class UBoxComponent *SupplyDropBox;

	UPROPERTY(VisibleAnywhere, Category = "SupplyDrop")
	class UStaticMeshComponent *DropMesh;

	UPROPERTY(VisibleAnywhere, Category = "SupplyDrop")
	class UParticleSystemComponent *SignalParticle;

	UPROPERTY()
	class AShooterCharacter *ShooterCharacter;

	UPROPERTY(VisibleAnywhere, Category = "SupplyDrop")
	class UWidgetComponent *OpenDropWidget;

	UPROPERTY(EditAnywhere, Category = "SupplyDrop")
	class UParticleSystem *DestroyDropParticles;

	UPROPERTY(EditAnywhere, Category = "SupplyDrop")
	class USoundCue *DestroyDropSound;

	UPROPERTY(EditAnywhere, Category = "SupplyDrop")
	TSubclassOf<class APickup> AmmoPickupClass;

	UPROPERTY()
	class ABuffPickup *BuffPickup;

	UPROPERTY()
	class AWeapon *Weapon;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<ABuffPickup>> BuffPickupClasses;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<AWeapon>> WeaponClasses;

protected:
	UFUNCTION()
	virtual void OnSupplyBoxBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION()
	virtual void OnSupplyBoxEndOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex);

	virtual void Destroyed() override;

	void SpawnItems();

	FVector RandomLocation();

};
