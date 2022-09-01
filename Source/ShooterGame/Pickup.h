// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

UCLASS()
class SHOOTERGAME_API APickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Pickups")
	class USphereComponent *PickupSphere;

	UPROPERTY(VisibleAnywhere, Category = "Pickups")
	class UStaticMeshComponent *PickupMesh;

	UPROPERTY(VisibleAnywhere, Category = "Pickups")
	class UNiagaraComponent *NiagaraPickup;

	UPROPERTY(VisibleAnywhere, Category = "Pickups")
	class AShooterCharacter *ShooterCharacter;

protected:
	virtual void OnPickupSphereBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
};
