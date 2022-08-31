// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "HitScanWeapon.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AHitScanWeapon : public AWeapon
{
	GENERATED_BODY()

public:
	// Overriding fire function from weapon class
	virtual void Fire(const FVector& HitTarget) override;
	
	
};
