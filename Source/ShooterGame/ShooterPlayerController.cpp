// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"
#include "ShooterHUD.h"
#include "CharacterOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "ShooterCharacter.h"
#include "Weapon.h"


void AShooterPlayerController::BeginPlay() 
{
    Super::BeginPlay();
    ShooterHUD = Cast<AShooterHUD>(GetHUD());
    ShooterCharacter = Cast<AShooterCharacter>(GetPawn());
}

void AShooterPlayerController::SetHUDHealth(float Health, float MaxHealth) 
{
    ShooterHUD = ShooterHUD == nullptr ? Cast<AShooterHUD>(GetHUD()) : ShooterHUD;
    if(ShooterHUD && ShooterHUD->CharacterOverlay && ShooterHUD->CharacterOverlay->HealthBar)
    {
        const float HealthPercent = Health / MaxHealth;
        ShooterHUD->CharacterOverlay->HealthBar->SetPercent(HealthPercent);
    }
}

void AShooterPlayerController::SetHUDMagazineAmmo(int32 MagazineAmmo) 
{
    ShooterHUD = ShooterHUD == nullptr ? Cast<AShooterHUD>(GetHUD()) : ShooterHUD;
    if(ShooterHUD && ShooterHUD->CharacterOverlay && ShooterHUD->CharacterOverlay->MagazineAmmo)
    {
        FString AmmoText = FString::Printf(TEXT("%d"), ShooterCharacter->GetEquippedWeapon()->GetMagazineAmmo());
        ShooterHUD->CharacterOverlay->MagazineAmmo->SetText(FText::FromString(AmmoText));
    }
}

void AShooterPlayerController::SetHUDCarriedAmmo(int32 CarriedAmmo) 
{
    ShooterHUD = ShooterHUD == nullptr ? Cast<AShooterHUD>(GetHUD()) : ShooterHUD;
    if(ShooterHUD && ShooterHUD->CharacterOverlay && ShooterHUD->CharacterOverlay->CarriedAmmo)
    {
        FString AmmoText = FString::Printf(TEXT("%d"), ShooterCharacter->GetEquippedWeapon()->GetCarriedAmmo());
        ShooterHUD->CharacterOverlay->CarriedAmmo->SetText(FText::FromString(AmmoText));
    }
}
