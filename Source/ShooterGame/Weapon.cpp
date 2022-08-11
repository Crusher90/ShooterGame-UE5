// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "ShooterCharacter.h"
#include "Blueprint/UserWidget.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);

	WeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponBox"));
	WeaponMesh->SetupAttachment(GetRootComponent());

	// PickupWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidget"));
	// PickupWidget->SetupAttachment(GetRootComponent());

	WeaponState = EWeaponState::EWS_Initial;
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	WeaponBox->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnWeaponBoxBeginOverlap);
	WeaponBox->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnWeaponBoxEndOverlap);
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::OnWeaponBoxBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if(OtherActor)
	{
		ShooterCharacter = ShooterCharacter == nullptr ? Cast<AShooterCharacter>(OtherActor) : ShooterCharacter;
		if(ShooterCharacter)
		{
			bOverlappingWeapon = true;
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
			bOverlappingWeapon = false;
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
	switch(WeaponState)
	{
		case EWeaponState::EWS_Initial:
			WeaponInitialState();
			break;

		case EWeaponState::EWS_Equipped:
			WeaponEquippedState();
			break;

		case EWeaponState::EWS_Dropped:
			WeaponDroppedState();
			break;
	}
}

void AWeapon::WeaponInitialState() 
{
	if(WeaponMesh)
	{
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		WeaponMesh->SetSimulatePhysics(true);
		WeaponMesh->SetEnableGravity(true);
		WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
		WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
		WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	}
	if(WeaponBox)
	{
		WeaponBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}

void AWeapon::WeaponEquippedState() 
{
	if (WeaponMesh)
	{
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		WeaponMesh->SetSimulatePhysics(false);
		WeaponMesh->SetEnableGravity(false);
		WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
		WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
		WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	}
	if (WeaponBox)
	{
		WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AWeapon::WeaponDroppedState() 
{
	SetWeaponState(EWeaponState::EWS_Initial);
}



