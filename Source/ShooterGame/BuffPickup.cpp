// Fill out your copyright notice in the Description page of Project Settings.

#include "BuffPickup.h"
#include "ShooterCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"
#include "Weapon.h"
#include "ShooterPlayerController.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "BuffTypes.h"


void ABuffPickup::OnPickupSphereBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
    if (OtherActor)
    {
        ShooterCharacter = Cast<AShooterCharacter>(OtherActor);
        if (ShooterCharacter)
        {
            // ShooterCharacter->SetBuffPickup(this);
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
        if (ShooterCharacter && ShooterCharacter->GetCharacterMovement())
        {
            ShooterCharacter->GetCharacterMovement()->JumpZVelocity = 1000.f;
            ShooterCharacter->GetShooterController()->SetHUDBuffText(*BuffTypes::JumpBuff);
        }
        break;

    case EBuffType::EBT_Speed:
        if(ShooterCharacter && ShooterCharacter->GetCharacterMovement())
        {
            ShooterCharacter->SetUseSprint(false);
            ShooterCharacter->GetCharacterMovement()->MaxWalkSpeed = 1000.f;
            ShooterCharacter->GetShooterController()->SetHUDBuffText(*BuffTypes::SpeedBuff);
        }
        break;

    case EBuffType::EBT_RapidFire:
        if (ShooterCharacter->GetEquippedWeapon() && (ShooterCharacter->GetEquippedWeapon()->GetWeaponType() == EWeaponType::EWT_AssaultRifle || ShooterCharacter->GetEquippedWeapon()->GetWeaponType() == EWeaponType::EWT_SMG))
        {
            ShooterCharacter->GetEquippedWeapon()->SetFireDelay(0.1f);
            ShooterCharacter->GetShooterController()->SetHUDBuffText(*BuffTypes::RapidFireBuff);
        }
        break;

    case EBuffType::EBT_Health:
        if(ShooterCharacter && ShooterCharacter->GetController())
        {
            ShooterCharacter->SetHealth(100.f);
            ShooterCharacter->GetShooterController()->SetHUDHealth(ShooterCharacter->GetHealth(), ShooterCharacter->GetMaxHealth());
            ShooterCharacter->GetShooterController()->SetHUDBuffText(*BuffTypes::HealthBuff);
        }
        break;
    }
    FTimerHandle RemoveBuffTimer;
    GetWorldTimerManager().SetTimer(RemoveBuffTimer, ShooterCharacter, &AShooterCharacter::InitialValues, RemoveBuffTime);
}

EBuffType ABuffPickup::RandomBuff() 
{
    int32 Value = FMath::RandRange(0, 3);
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

    case 3:
        BuffType = EBuffType::EBT_Health;
        break;
    }
    return BuffType;
}

  