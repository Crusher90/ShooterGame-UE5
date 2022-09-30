// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieSpawner.h"
#include "Components/BoxComponent.h"
#include "Enemy.h"
#include "TimerManager.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AZombieSpawner::AZombieSpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SpawnBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnBox"));
	SetRootComponent(SpawnBox);
	SpawnBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SpawnBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}

// Called when the game starts or when spawned
void AZombieSpawner::BeginPlay()
{
	Super::BeginPlay();

	BoxOrigin = SpawnBox->GetComponentLocation();
	BoxExtent = SpawnBox->GetScaledBoxExtent();
	StartTimer();
}

// Called every frame
void AZombieSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AZombieSpawner::SpawnZombie()
{
	int32 TypesOfZombies = EnemyClasses.Num();
	if (TypesOfZombies > 0)
	{
		int32 RandValue = FMath::RandRange(0, TypesOfZombies - 1);
		for (int32 i = 0; i < ZombiesToSpawn; i++)
		{
			Enemy = GetWorld()->SpawnActor<AEnemy>(EnemyClasses[RandValue], UKismetMathLibrary::RandomPointInBoundingBox(BoxOrigin, BoxExtent), FRotator::ZeroRotator);
		}
		ZombiesToSpawn = FMath::RandRange(MinZombieSpawn, MaxZombieSpawn);
		ZombieSpawnTime = FMath::RandRange(MinSpawnTime, MaxSpawnTime);
		StartTimer();
	}
}

void AZombieSpawner::StartTimer()
{
	GetWorldTimerManager().SetTimer(ZombieSpawnTimer, this, &ThisClass::SpawnZombie, ZombieSpawnTime);
}
