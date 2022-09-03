// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ShooterPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AShooterPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	void SetHUDHealth(float Health, float MaxHealth);

	void SetHUDMagazineAmmo(int32 MagazineAmmo);

	void SetHUDCarriedAmmo(int32 CarriedAmmo);

private:
	UPROPERTY()
	class AShooterHUD *ShooterHUD;

	UPROPERTY()
	class AShooterCharacter *ShooterCharacter;
};
