// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "WeaponTypes.h"
#include "AmmoPickup.generated.h"

/**
 *
 */
UCLASS()
class SHOOTERGAME_API AAmmoPickup : public APickup
{
	GENERATED_BODY()
	
protected:
	virtual void OnPickupSphereBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult) override;

public:
	UPROPERTY(EditAnywhere, Category = "AmmoPickups")
	EWeaponType WeaponType;
};
