// Fill out your copyright notice in the Description page of Project Settings.


#include "BuffPickup.h"
#include "ShooterCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"
#include "Weapon.h"


void ABuffPickup::OnPickupSphereBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult) 
{
    if(OtherActor)
    {
        ShooterCharacter = Cast<AShooterCharacter>(OtherActor);
        if(ShooterCharacter)
        {
            SetBuffState(BuffType);
        }
    }
    Super::OnPickupSphereBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void ABuffPickup::SetBuffState(EBuffType BuffTypeValue) 
{
    if(ShooterCharacter == nullptr && ShooterCharacter->GetCharacterMovement() == nullptr)
        return;
    switch (BuffType)
    {
    case EBuffType::EBT_Jump :
        UE_LOG(LogTemp, Warning, TEXT("Jump"));
        ShooterCharacter->GetCharacterMovement()->JumpZVelocity = 2000.f;
        break;

    case EBuffType::EBT_Speed :
        UE_LOG(LogTemp, Warning, TEXT("Speed"));
        ShooterCharacter->SetUseSprint(false);
        ShooterCharacter->GetCharacterMovement()->MaxWalkSpeed = 3000.f;
        break;

    case EBuffType::EBT_RapidFire :
        if(ShooterCharacter->GetEquippedWeapon() && (ShooterCharacter->GetEquippedWeapon()->GetWeaponType() == EWeaponType::EWT_AssaultRifle || ShooterCharacter->GetEquippedWeapon()->GetWeaponType() == EWeaponType::EWT_SMG))
        {
            ShooterCharacter->GetEquippedWeapon()->SetFireDelay(0.1f);
        }
    }
    FTimerHandle RemoveBuffTimer;
    GetWorldTimerManager().SetTimer(RemoveBuffTimer, ShooterCharacter, &AShooterCharacter::InitialValues, RemoveBuffTime);
}

    