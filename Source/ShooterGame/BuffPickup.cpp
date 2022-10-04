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
            SetBuffState(BuffType);
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
            GetWorldTimerManager().SetTimer(RemoveJumpTimer, ShooterCharacter, &AShooterCharacter::ResetJumpBuff, RemoveBuffTime);
            ShooterCharacter->GetCharacterMovement()->JumpZVelocity = 1000.f;
            ShooterCharacter->GetShooterController()->SetHUDBuffText(*BuffTypes::JumpBuff);
        }
        break;

    case EBuffType::EBT_Speed:
        if(ShooterCharacter && ShooterCharacter->GetCharacterMovement())
        {
            GetWorldTimerManager().SetTimer(RemoveSpeedTimer, ShooterCharacter, &AShooterCharacter::ResetSpeedBuff, RemoveBuffTime);
            ShooterCharacter->SetUseSprint(false);
            ShooterCharacter->GetCharacterMovement()->MaxWalkSpeed = 1000.f;
            ShooterCharacter->GetShooterController()->SetHUDBuffText(*BuffTypes::SpeedBuff);
        }
        break;

    case EBuffType::EBT_RapidFire:
        if (ShooterCharacter->GetEquippedWeapon() && (ShooterCharacter->GetEquippedWeapon()->GetWeaponType() == EWeaponType::EWT_AssaultRifle || ShooterCharacter->GetEquippedWeapon()->GetWeaponType() == EWeaponType::EWT_SMG))
        {
            GetWorldTimerManager().SetTimer(RemoveRapidFireTimer, ShooterCharacter, &AShooterCharacter::ResetRapidFireBuff, RemoveBuffTime);
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
}

  