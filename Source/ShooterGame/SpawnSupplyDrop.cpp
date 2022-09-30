// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnSupplyDrop.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "SupplyDrop.h"

// Sets default values
ASpawnSupplyDrop::ASpawnSupplyDrop()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SpawnBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnBox"));
	SetRootComponent(SpawnBox);
	SpawnBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SpawnBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}

// Called when the game starts or when spawned
void ASpawnSupplyDrop::BeginPlay()
{
	Super::BeginPlay();

	BoxExtent = SpawnBox->GetScaledBoxExtent();
	BoxOrigin = SpawnBox->GetComponentLocation();
	SpawnSupplyDrop();
	// StartSupplyDropTimer();
}

// Called every frame
void ASpawnSupplyDrop::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpawnSupplyDrop::SpawnSupplyDrop() 
{
	if(SupplyDropClass)
	{
		SupplyDrop = GetWorld()->SpawnActor<ASupplyDrop>(SupplyDropClass, UKismetMathLibrary::RandomPointInBoundingBox(BoxOrigin, BoxExtent), GetActorRotation());
	}
	StartSupplyDropTimer();
}

void ASpawnSupplyDrop::StartSupplyDropTimer() 
{
	int32 SpawnDropTime = FMath::RandRange(MinSpawnDropTime, MaxSpawnDropTime);
	GetWorldTimerManager().SetTimer(SpawnDropTimer, this, &ThisClass::SpawnSupplyDrop, SpawnDropTime);
}

