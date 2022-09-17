// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnSupplyDrop.generated.h"

UCLASS()
class SHOOTERGAME_API ASpawnSupplyDrop : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnSupplyDrop();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, Category = "SupplyDrop")
	class UBoxComponent *SpawnBox;

	FTimerHandle SpawnDropTimer;

	UPROPERTY(EditAnywhere, Category = "SupplyDrop")
	float MinSpawnDropTime;

	UPROPERTY(EditAnywhere, Category = "SupplyDrop")
	float MaxSpawnDropTime;

	UPROPERTY(EditAnywhere, Category = "SupplyDrop")
	TSubclassOf<class ASupplyDrop> SupplyDropClass;

	UPROPERTY()
	class ASupplyDrop *SupplyDrop;

	FVector BoxExtent;

	FVector BoxOrigin;

protected:
	void SpawnSupplyDrop();

	void StartSupplyDropTimer();
};
