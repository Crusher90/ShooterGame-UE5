// Fill out your copyright notice in the Description page of Project Settings.


#include "HitScanWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"


void AHitScanWeapon::Fire(const FVector& HitTarget) 
{
    Super::Fire(HitTarget);

    if(BulletFireSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, BulletFireSound, GetActorLocation());
    }
    if(BeamParticles)
    {
        Beam = UGameplayStatics::SpawnEmitterAtLocation(this, BeamParticles, GetWeaponMesh()->GetSocketTransform(FName("MuzzleFlash")).GetLocation());
        if(Beam)
        {
            Beam->SetVectorParameter(FName("Target"), HitTarget);
        }
    }
}
