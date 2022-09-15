// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CombatState.h"
#include "TurningInPlace.h"
#include "WeaponTypes.h"
// #include "BuffPickup.h"
#include "ShooterCharacter.generated.h"

#define TRACE_LENGTH 100000.f

UCLASS()
class SHOOTERGAME_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;

private:
	// Declaring SpringArmComponent
	UPROPERTY(VisibleAnywhere, Category = "ShooterCharacter")
	class USpringArmComponent *CameraBoom;

	// Declaring ComponentComponent
	UPROPERTY(VisibleAnywhere, Category = "ShooterCharacter")
	class UCameraComponent *FollowCamera;

	// Weapon That We Are Currently Overlapping with
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ShooterCharacter", meta = (AllowPrivateAccess = "true"))
	class AWeapon *OverlappingWeapon;

	// Currently Equipped Weapon in our Hands
	UPROPERTY(VisibleAnywhere, Category = "ShooterCharacter")
	class AWeapon *EquippedWeapon;

	// Game Overall Sensitivity
	UPROPERTY(EditAnywhere, Category = "CharacterProperties")
	float Sensitivity = 45.f;

	// ENUM Combat State Variable
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ShooterCharacter", meta = (AllowPrivateAccess = "true"))
	ECombatState CombatState = ECombatState::ECS_Unoccupied;

	// ENUM TurningInPlace State Variable
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ShooterCharacter", meta = (AllowPrivateAccess = "true"))
	ETurningInPlace TurningInPlace = ETurningInPlace::ETIP_NotTurning;

	FRotator StartingAimRotation;

	float InterpAO_Yaw;

	float AO_Yaw;

	float AO_Pitch;

	UPROPERTY()
	class AShooterPlayerController *ShooterController;

	UPROPERTY()
	class AShooterHUD *ShooterHUD;

	// Montages
	UPROPERTY(EditAnywhere, Category = "ShooterCharacter")
	class UAnimMontage *FireRifleMontage;

	UPROPERTY(EditAnywhere, Category = "ShooterCharacter")
	class UAnimMontage *ReloadMontage;

	FVector HitTarget;

	FTimerHandle FireTimer;

	UPROPERTY(VisibleAnywhere)
	bool bCanFire = true;

	bool bFireButtonPressed = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ShooterCharacter", meta = (AllowPrivateAccess = "true"))
	float Health = 100.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ShooterCharacter", meta = (AllowPrivateAccess = "true"))
	float MaxHealth = 100.f;

	UPROPERTY(VisibleAnywhere, Category = "ShooterCharacter")
	bool bUseSprint = true;

	UPROPERTY(VisibleAnywhere)
	class AEnemy *Enemy;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ShooterCharacter", meta = (AllowPrivateAccess = "true"))
	bool bDie = false;

	UPROPERTY(VisibleAnywhere, Category = "ShooterCharacter")
	bool bAim = false;

	UPROPERTY(EditAnywhere, Category = "ShooterCharacter")
	float ZoomInterpSpeed = 20.f;

	UPROPERTY(EditAnywhere, Category = "ShooterCharacter")
	float ZoomedFOV;

	float DefaultFOV;

	float CurrentFOV;

	UPROPERTY()
	class ASupplyDrop *SupplyDrop;

	UPROPERTY(EditAnywhere, Category = "SupplyDrop")
	float SupplyOpenTime = 5.f;

	// UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ShooterCharacter", meta = (AllowPrivateAccess = "true"))
	// class ABuffPickup *BuffPickup;

protected:
	// Move Forward Function To Move Character In World Forward Direction X
	void MoveForward(float Value);

	// Move Right Function To Move Character In World Forward Direction Y
	void MoveRight(float Value);

	// Look Up Function To Look Character In World Pitch Direction Y
	void LookUp(float Value);

	// Look Right Function To Move Character In World Yaw Direction Z
	void LookRight(float Value);

	// Crouch Function To Set Character State To Crouch
	void CrouchButtonPressed();

	// Jump Functionality For Character
	void CharacterJump();

	// Sprint Functionality for Character
	void Sprint();
	void StopSprint();

	// Function Call When E Key Is Pressed
	void EquipButtonPressed();

	// Equip Weapon Function
	void EquipWeapon(AWeapon *WeaponToEquip);

	// Function For Swapping Weapons
	void SwapWeapons();

	// Function Call Fire Button is Pressed
	void FireButtonPressed();

	// Function Call Fire Button is Pressed
	void FireButtonReleased();

	void AimButtonPressed();

	void AimOffset(float DeltaTime);

	void TurnInPlace(float DeltaTime);

	void PlayFireRifleMontage();

	void SetCrosshairToScreen(float DeltaTime);

	void TraceUnderCrosshair(FHitResult &TraceHitResult);

	void FireTimerStart();

	void FireTimerFinished();

	void ReloadButtonPressed();

	void AfterReloadMontage();

	void ReloadWeapon();

	void SetMagazineHUDAmmo();

	void SetCarriedHUDAmmo();

	void Die();

	void Aim(float DeltaTime);

	void FButtonPressed();

	void FButtonReleased();

public:
	// Weapon Equipping Functionality
	void AttachWeaponToHands(AWeapon *WeaponInHands);

	// Weapon Drop Functionality
	void DropWeaponFromHands(AWeapon *WeaponToDrop);

	void InitialValues();

	virtual float TakeDamage(float DamageAmount, FDamageEvent const &DamageEvent, AController *EventInstigator, AActor *DamageCauser) override;

	UFUNCTION(BlueprintImplementableEvent)
	void ShowSniperScopeWidget(bool bShowScope);

	// Variables
	FTimerHandle SupplyOpenTimer;

public:
	/* ******************Getters And Setters****************** */

	// Combat State
	FORCEINLINE ECombatState GetCombatState() const { return CombatState; }
	FORCEINLINE void SetCombatState(ECombatState State) { CombatState = State; }

	// Combat State
	FORCEINLINE ETurningInPlace GetTurningInPlace() const { return TurningInPlace; }
	FORCEINLINE void SetTurningInPlace(ETurningInPlace State) { TurningInPlace = State; }

	// Overlapping Weapon
	FORCEINLINE AWeapon *GetOverlappingWeapon() { return OverlappingWeapon; }
	FORCEINLINE void SetOverlappingWeapon(AWeapon *WeaponToSet) { OverlappingWeapon = WeaponToSet; }

	// Equipped Weapon
	FORCEINLINE AWeapon *GetEquippedWeapon() { return EquippedWeapon; }
	FORCEINLINE void SetEquippedWeapon(AWeapon *WeaponToSet) { EquippedWeapon = WeaponToSet; }

	FORCEINLINE float GetAO_Yaw() const {return AO_Yaw;}
	FORCEINLINE float GetAO_Pitch() const {return AO_Pitch;}

	FORCEINLINE FVector GetHitTarget() const { return HitTarget; }

	FORCEINLINE bool GetUseSprint() const {return bUseSprint;}
	FORCEINLINE void SetUseSprint(bool Value) {bUseSprint = Value;}

	FORCEINLINE AEnemy *GetEnemy() const {return Enemy;}
	FORCEINLINE bool GetDie() const {return bDie;}

	FORCEINLINE AShooterPlayerController *GetShooterController() const { return ShooterController; }
	FORCEINLINE void SetSupplyDrop(ASupplyDrop *Value) { SupplyDrop = Value; }

	FORCEINLINE float GetHealth() const {return Health;}
	FORCEINLINE void SetHealth(float Value) {Health = Value;}

	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }

	// FORCEINLINE ABuffPickup *GetBuffPickup() const { return BuffPickup; }
	// FORCEINLINE void SetBuffPickup(ABuffPickup *Value) { BuffPickup = Value; }
};
