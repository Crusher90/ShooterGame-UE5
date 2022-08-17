// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "ProjectileWeapon.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AProjectileWeapon : public AWeapon
{
	GENERATED_BODY()

public:
	// Overriding fire function from weapon class
	virtual void Fire() override;

private:
	UPROPERTY(EditAnywhere, Category = "ProjectileWeapon")
	TSubclassOf<class AProjectileBase> ProjectileToSpawn;
	
};
