// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoPickup.h"
#include "ShooterCharacter.h"
#include "Weapon.h"


void AAmmoPickup::OnPickupSphereBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult) 
{
    if(OtherActor)
    {
        ShooterCharacter = Cast<AShooterCharacter>(OtherActor);
        if(ShooterCharacter == nullptr && ShooterCharacter->GetEquippedWeapon() == nullptr)
            return;
        if(ShooterCharacter && ShooterCharacter->GetEquippedWeapon())
        {
            ShooterCharacter->GetEquippedWeapon()->CarriedAmmo += 60;
            Super::OnPickupSphereBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
        }
    }
}
