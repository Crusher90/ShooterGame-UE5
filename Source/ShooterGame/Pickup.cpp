// Fill out your copyright notice in the Description page of Project Settings.

#include "Pickup.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "ShooterCharacter.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Weapon.h"
#include "ShooterPlayerController.h"

// Sets default values
APickup::APickup()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PickupSphere = CreateDefaultSubobject<USphereComponent>(TEXT("PickupSphere"));
	SetRootComponent(PickupSphere);
	PickupSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	PickupSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	PickupSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	PickupMesh->SetupAttachment(GetRootComponent());
	PickupMesh->SetRenderCustomDepth(true);
	PickupMesh->SetCustomDepthStencilValue(250);
	PickupMesh->MarkRenderStateDirty();

	NiagaraPickup = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraPickup"));
	NiagaraPickup->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PickupSphere)
	{
		PickupSphere->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnPickupSphereBeginOverlap);
	}
}

void APickup::OnPickupSphereBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (OtherActor)
	{
		ShooterCharacter = Cast<AShooterCharacter>(OtherActor);
		if (ShooterCharacter)
		{
			Destroy();
			if(ShooterCharacter->GetEquippedWeapon())
			{
				ShooterCharacter->GetShooterController()->SetHUDCarriedAmmo(ShooterCharacter->GetEquippedWeapon()->GetCarriedAmmo());
			}
		}
	}
}

void APickup::Destroyed()
{
	Super::Destroyed();

	if (AfterPickupSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, AfterPickupSound, GetActorLocation());
	}
	if (AfterNiagaraParticles)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, AfterNiagaraParticles, GetActorLocation());
	}
	if (AfterPickupParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, AfterPickupParticles, GetActorLocation());
	}
}
