// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "WeaponTypes.h"
#include "BuffPickup.generated.h"

UENUM(BlueprintType)
enum class EBuffType : uint8
{
	EBT_Jump UMETA(DisplayName = "JumpBuff"),
	EBT_Speed UMETA(DisplayName = "SpeedBuff"),
	EBT_Health UMETA(DisplayName = "HealthBuff"),
	EBT_RapidFire UMETA(DisplayName = "RapidFire"),

	EBT_Max UMETA(DisplayName = "DefaultMax")
};

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API ABuffPickup : public APickup
{
	GENERATED_BODY()

protected:
	virtual void OnPickupSphereBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult) override;

	void SetBuffState(EBuffType BuffTypeValue);

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BuffPickups", meta = (AllowPrivateAccess = "true"))
	EBuffType BuffType;

	FTimerHandle RemoveJumpTimer;

	FTimerHandle RemoveSpeedTimer;

	FTimerHandle RemoveRapidFireTimer;
	
	UPROPERTY(EditAnywhere, Category = "BuffPickups")
	float RemoveBuffTime = 10.f;

	EWeaponType WeaponType;
};
  