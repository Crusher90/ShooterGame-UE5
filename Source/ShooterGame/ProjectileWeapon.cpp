// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileWeapon.h"
#include "ProjectileBase.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"


void AProjectileWeapon::Fire() 
{
    Super::Fire();

    const USkeletalMeshSocket *MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName(FName("MuzzleFlash"));
    if(MuzzleFlashSocket)
    {
        FActorSpawnParameters SpawnParameters;
        if(ProjectileToSpawn)
        {
            GetWorld()->SpawnActor<AProjectileBase>(ProjectileToSpawn, MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh()), SpawnParameters);
            UGameplayStatics::PlaySoundAtLocation(this, BulletFireSound, GetActorLocation());
        } 
    }
}
