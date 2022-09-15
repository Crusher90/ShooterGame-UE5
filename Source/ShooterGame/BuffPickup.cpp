// Fill out your copyright notice in the Description page of Project Settings.

#include "BuffPickup.h"
#include "ShooterCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"
#include "Weapon.h"


void ABuffPickup::OnPickupSphereBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
    if (OtherActor)
    {
        ShooterCharacter = Cast<AShooterCharacter>(OtherActor);
        if (ShooterCharacter)
        {
            SetBuffState(RandomBuff());
        }
    }
    Super::OnPickupSphereBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void ABuffPickup::SetBuffState(EBuffType BuffTypeValue)
{
    if (ShooterCharacter == nullptr && ShooterCharacter->GetCharacterMovement() == nullptr)
        return;
    switch (BuffType)
    {
    case EBuffType::EBT_Jump:
        ShooterCharacter->GetCharacterMovement()->JumpZVelocity = 1000.f;
        break;

    case EBuffType::EBT_Speed:
        ShooterCharacter->SetUseSprint(false);
        ShooterCharacter->GetCharacterMovement()->MaxWalkSpeed = 1000.f;
        break;

    case EBuffType::EBT_RapidFire:
        if (ShooterCharacter->GetEquippedWeapon() && (ShooterCharacter->GetEquippedWeapon()->GetWeaponType() == EWeaponType::EWT_AssaultRifle || ShooterCharacter->GetEquippedWeapon()->GetWeaponType() == EWeaponType::EWT_SMG))
        {
            ShooterCharacter->GetEquippedWeapon()->SetFireDelay(0.1f);
        }
    }
    FTimerHandle RemoveBuffTimer;
    GetWorldTimerManager().SetTimer(RemoveBuffTimer, ShooterCharacter, &AShooterCharacter::InitialValues, RemoveBuffTime);
}

EBuffType ABuffPickup::RandomBuff() 
{
    int32 Value = FMath::RandRange(0, 2);
    switch (Value)
    {
    case 0:
        BuffType = EBuffType::EBT_Jump;
        break;
    
    case 1:
        BuffType = EBuffType::EBT_RapidFire;
        break;

    case 2:
        BuffType = EBuffType::EBT_Speed;
        break;
    }
    return BuffType;
}

