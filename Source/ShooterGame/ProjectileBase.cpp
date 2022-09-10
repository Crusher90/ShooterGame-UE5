// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectileBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "ShooterCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ProjectileBox = CreateDefaultSubobject<UBoxComponent>(TEXT("ProjectileBox"));
	SetRootComponent(ProjectileBox);
	ProjectileBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ProjectileBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	ProjectileBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	ProjectileBox->SetEnableGravity(true);

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetupAttachment(GetRootComponent());
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->InitialSpeed = 2000.f;
	ProjectileMovementComponent->MaxSpeed = 2000.f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;

	BulletTracer = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("BulletTracer"));
	BulletTracer->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	ProjectileBox->OnComponentHit.AddDynamic(this, &ThisClass::OnProjectileHit);
}

// Called every frame
void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectileBase::OnProjectileHit(UPrimitiveComponent *HitComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit)
{
	if (WeaponType == EWeaponType::EWT_GrenadeLauncher)
	{
		GetWorldTimerManager().SetTimer(GrenadeDestroyTimer, this, &ThisClass::DestroyProjectile, GrenadeDestroyTime);
		return;
	}
	DestroyProjectile();
}

void AProjectileBase::DestroyProjectile()
{
	Destroy();
}

void AProjectileBase::ExplodeDamage() 
{
	if (WeaponType == EWeaponType::EWT_GrenadeLauncher || WeaponType == EWeaponType::EWT_RocketLauncher)
	{
		UGameplayStatics::ApplyRadialDamageWithFalloff(this, 200.f, 80.f, GetActorLocation(), 200.f, 400.f, 1.f, UDamageType::StaticClass(), TArray<AActor *>(), this);
	}
}

void AProjectileBase::Destroyed()
{
	Super::Destroyed();

	if (BulletImpactParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BulletImpactParticles, GetActorLocation());
	}
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
	}
	ExplodeDamage();
}
