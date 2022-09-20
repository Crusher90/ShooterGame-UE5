// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZombieSpawner.generated.h"

UCLASS()
class SHOOTERGAME_API AZombieSpawner : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AZombieSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, Category = "ZombieSpawner")
	class UBoxComponent *SpawnBox;

	FTimerHandle ZombieSpawnTimer;

	UPROPERTY(VisibleAnywhere, Category = "ZombieSpawner")
	float ZombieSpawnTime = 10.f;

	UPROPERTY(EditAnywhere, Category = "ZombieSpawner")
	TArray<TSubclassOf<class AEnemy>> EnemyClasses;

	UPROPERTY()
	AEnemy *Enemy;

	UPROPERTY(VisibleAnywhere, Category = "ZombieSpawner")
	int32 ZombiesToSpawn = 1;

	FVector BoxOrigin;

	FVector BoxExtent;

	UPROPERTY(EditAnywhere, Category = "ZombieSpawner");
	int32 MinZombieSpawn = 10;

	UPROPERTY(EditAnywhere, Category = "ZombieSpawner");
	int32 MaxZombieSpawn = 10;

	UPROPERTY(EditAnywhere, Category = "ZombieSpawner");
	float MinSpawnTime = 10;

	UPROPERTY(EditAnywhere, Category = "ZombieSpawner");
	float MaxSpawnTime = 10;

protected:
	void SpawnZombie();

	void ResetTimer();

	void StartTimer();
};
