// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileWeapon.h"
#include "ProjectileBase.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"


void AProjectileWeapon::Fire(const FVector& HitTarget) 
{
    Super::Fire(HitTarget);

    const USkeletalMeshSocket *MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName(FName("MuzzleFlash"));
    if(MuzzleFlashSocket)
    {
        FVector ToTarget = HitTarget - MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh()).GetLocation();
        if(ProjectileToSpawn)
        {
            FActorSpawnParameters SpawnParameters;
            GetWorld()->SpawnActor<AProjectileBase>(ProjectileToSpawn, MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh()).GetLocation(), ToTarget.Rotation(), SpawnParameters);
            UGameplayStatics::PlaySoundAtLocation(this, BulletFireSound, GetActorLocation());
        } 
    }
}
