// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "ShooterCharacter.h"
#include "Blueprint/UserWidget.h"
#include "ProjectileWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Particles/ParticleSystem.h"
#include "Enemy.h"
#include "ShooterPlayerController.h"
#include "Animation/AnimationAsset.h"

// Sets default values
AWeapon::AWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);
	WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	WeaponMesh->SetSimulatePhysics(false);
	WeaponMesh->SetEnableGravity(false);
	WeaponMesh->SetRenderCustomDepth(true);
	WeaponMesh->SetCustomDepthStencilValue(251);
	WeaponMesh->MarkRenderStateDirty();

	WeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponBox"));
	WeaponBox->SetupAttachment(GetRootComponent());
	WeaponBox->SetBoxExtent(FVector(50.f));
	WeaponBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	WeaponBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	WeaponBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	PickupWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidget"));
	PickupWidget->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	WeaponBox->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnWeaponBoxBeginOverlap);
	WeaponBox->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnWeaponBoxEndOverlap);
	PickupWidget->SetVisibility(false);
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeapon::OnWeaponBoxBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (OtherActor)
	{
		ShooterCharacter = ShooterCharacter == nullptr ? Cast<AShooterCharacter>(OtherActor) : ShooterCharacter;
		if (ShooterCharacter)
		{
			ShooterCharacter->SetOverlappingWeapon(this);
			PickupWidget->SetVisibility(true);
		}
	}
}

void AWeapon::OnWeaponBoxEndOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		ShooterCharacter = ShooterCharacter == nullptr ? Cast<AShooterCharacter>(OtherActor) : ShooterCharacter;
		if (ShooterCharacter)
		{
			ShooterCharacter->SetOverlappingWeapon(nullptr);
			PickupWidget->SetVisibility(false);
		}
	}
}

void AWeapon::SetWeaponState(EWeaponState State)
{
	WeaponState = State;
	OnWeaponStateSet();
}

void AWeapon::OnWeaponStateSet()
{
	switch (WeaponState)
	{
	case EWeaponState::EWS_Equipped:
		WeaponEquippedState();
		break;

	case EWeaponState::EWS_Dropped:
		WeaponDroppedState();
		break;
	}
}

void AWeapon::WeaponEquippedState()
{
	if (WeaponBox)
	{
		WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	if (WeaponMesh)
	{
		WeaponMesh->SetSimulatePhysics(false);
		WeaponMesh->SetEnableGravity(false);
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		WeaponMesh->SetRenderCustomDepth(false);
		WeaponMesh->MarkRenderStateDirty();
	}
}

void AWeapon::WeaponDroppedState()
{
	if (WeaponMesh)
	{
		WeaponMesh->SetSimulatePhysics(true);
		WeaponMesh->SetEnableGravity(true);
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
		WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
		WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
		WeaponMesh->SetRenderCustomDepth(true);
		WeaponMesh->MarkRenderStateDirty();
	}
	FTimerHandle DelayTimer;
	GetWorldTimerManager().SetTimer(DelayTimer, this, &ThisClass::DelayedDrop, 1.2f);
}

void AWeapon::DelayedDrop()
{
	if (WeaponBox)
	{
		WeaponBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		WeaponBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		WeaponBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	}
}

void AWeapon::Fire(const FVector &HitTarget)
{
	Enemy = ShooterCharacter->GetEnemy();
	const USkeletalMeshSocket *MuzzleFlashSocket = WeaponMesh->GetSocketByName("MuzzleFlash");
	if (MuzzleFlashSocket && MuzzleFlashParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, MuzzleFlashParticle, MuzzleFlashSocket->GetSocketLocation(WeaponMesh), MuzzleFlashSocket->GetSocketTransform(WeaponMesh).GetRotation().Rotator());
	}
	if (HitParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, HitParticle, HitTarget);
	}
	if (WeaponMesh)
	{
		WeaponMesh->PlayAnimation(GunFireAnimation, false);
	}
	if (MagazineAmmo > 0)
		--MagazineAmmo;
	else
		MagazineAmmo = 0;
	if (Enemy && !Enemy->GetDie())
	{
		ApplyDamage();
	}
}

void AWeapon::ApplyDamage()
{
	if (WeaponType == EWeaponType::EWT_AssaultRifle || WeaponType == EWeaponType::EWT_Pistol || WeaponType == EWeaponType::EWT_SMG || WeaponType == EWeaponType::EWT_SniperRifle)
	{
		if (ShooterCharacter && Enemy)
		{
			if (Enemy->GetBone() == FString("Spine2") || Enemy->GetBone() == FString("Head"))
			{
				UGameplayStatics::ApplyDamage(Enemy, HeadShotDamage, ShooterController, this, UDamageType::StaticClass());
			}
			else
			{
				UGameplayStatics::ApplyDamage(Enemy, Damage, ShooterController, this, UDamageType::StaticClass());
			}
		}
	}
}

void AWeapon::Reload()
{
	int32 AmountToReload = MagazineSize - MagazineAmmo;
	if (AmountToReload >= CarriedAmmo)
	{
		MagazineAmmo = MagazineAmmo + CarriedAmmo;
		CarriedAmmo = 0;
	}
	else
	{
		CarriedAmmo = CarriedAmmo - AmountToReload;
		MagazineAmmo = MagazineAmmo + AmountToReload;
	}
}
