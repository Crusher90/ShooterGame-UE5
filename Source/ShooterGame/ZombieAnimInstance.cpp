// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieAnimInstance.h"
#include "Enemy.h"


void UZombieAnimInstance::NativeInitializeAnimation() 
{
    Super::NativeInitializeAnimation();
    if(Enemy == nullptr)
    {
        Enemy = Cast<AEnemy>(TryGetPawnOwner());
    }
}

void UZombieAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
    Super::NativeUpdateAnimation(DeltaTime);
    if (Enemy == nullptr)
    {
        Enemy = Cast<AEnemy>(TryGetPawnOwner());
    }
    if(Enemy)
    {
        FVector Velocity = Enemy->GetVelocity();
        Velocity.Z = 0.f;
        Speed = Velocity.Size();
    }
}
