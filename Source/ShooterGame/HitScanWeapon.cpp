// Fill out your copyright notice in the Description page of Project Settings.


#include "HitScanWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"


void AHitScanWeapon::Fire(const FVector& HitTarget) 
{
    Super::Fire(HitTarget);

    if(BulletFireSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, BulletFireSound, GetActorLocation());
    }
}
