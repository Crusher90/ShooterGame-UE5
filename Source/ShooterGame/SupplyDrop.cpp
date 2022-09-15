// Fill out your copyright notice in the Description page of Project Settings.

#include "SupplyDrop.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "ShooterCharacter.h"
#include "Components/WidgetComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Pickup.h"
#include "BuffPickup.h"
#include "Weapon.h"

// Sets default values
ASupplyDrop::ASupplyDrop()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SupplyDropBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SupplyDropBox"));
	SetRootComponent(SupplyDropBox);
	SupplyDropBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SupplyDropBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SupplyDropBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	SupplyDropBox->SetSimulatePhysics(true);
	SupplyDropBox->SetLinearDamping(20.f);

	DropMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DropMesh"));
	DropMesh->SetupAttachment(GetRootComponent());

	SignalParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SignalParticle"));
	SignalParticle->SetupAttachment(GetRootComponent());

	OpenDropWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OpenDropWidget"));
	OpenDropWidget->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ASupplyDrop::BeginPlay()
{
	Super::BeginPlay();

	SupplyDropBox->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnSupplyBoxBeginOverlap);
	SupplyDropBox->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnSupplyBoxEndOverlap);
	OpenDropWidget->SetVisibility(false);
}

// Called every frame
void ASupplyDrop::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASupplyDrop::OnSupplyBoxBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (OtherActor)
	{
		ShooterCharacter = Cast<AShooterCharacter>(OtherActor);
		if (ShooterCharacter)
		{
			ShooterCharacter->SetSupplyDrop(this);
			OpenDropWidget->SetVisibility(true);
		}
	}
}

void ASupplyDrop::OnSupplyBoxEndOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		ShooterCharacter = Cast<AShooterCharacter>(OtherActor);
		if (ShooterCharacter)
		{
			ShooterCharacter->SetSupplyDrop(nullptr);
			GetWorldTimerManager().ClearTimer(ShooterCharacter->SupplyOpenTimer);
			OpenDropWidget->SetVisibility(false);
		}
	}
}

void ASupplyDrop::Destroyed()
{
	Super::Destroyed();
	if (DestroyDropParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, DestroyDropParticles, GetActorLocation());
	}
	if (DestroyDropSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DestroyDropSound, GetActorLocation());
	}
}

void ASupplyDrop::SpawnItems()
{
	int32 NumBuffPickupClass = BuffPickupClasses.Num();
	if (NumBuffPickupClass > 0)
	{
		int32 Value = FMath::RandRange(0, NumBuffPickupClass - 1);
		BuffPickup = GetWorld()->SpawnActor<ABuffPickup>(BuffPickupClasses[Value], GetActorLocation() + RandomLocation(), GetActorRotation());
	}
	int32 NumWeaponClass = WeaponClasses.Num();
	if (NumWeaponClass > 0)
	{
		int32 Value = FMath::RandRange(0, NumWeaponClass - 1);
		Weapon = GetWorld()->SpawnActor<AWeapon>(WeaponClasses[Value], GetActorLocation() + RandomLocation(), GetActorRotation());
	}
	if (AmmoPickupClass)
	{
		for (int i = 0; i < 4; i++)
		{
			GetWorld()->SpawnActor<APickup>(AmmoPickupClass, GetActorLocation() + RandomLocation(), GetActorRotation());
			UE_LOG(LogTemp, Warning, TEXT("SpawnPickups"));
		}
	}
}

FVector ASupplyDrop::RandomLocation()
{
	FVector RandomLocation;
	RandomLocation.X = FMath::RandRange(-100.f, 100.f);
	RandomLocation.Y = FMath::RandRange(-100.f, 100.f);

	return RandomLocation;
}

void ASupplyDrop::DestroyActor()
{
	SpawnItems();
	Destroy();
}
