// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CombatState.h"
#include "ShooterCharacter.generated.h"

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
	UPROPERTY(VisibleAnywhere, Category = "ShooterCharacter")
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

	FRotator StartingAimRotation;

	float AO_Yaw;

	float AO_Pitch;

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

	void AimOffset(float DeltaTime);

public:
	// Weapon Equipping Functionality
	void AttachWeaponToHands(AWeapon *WeaponInHands);

	// Weapon Drop Functionality
	void DropWeaponFromHands(AWeapon *WeaponToDrop);

public:
	/* ******************Getters And Setters****************** */

	// Combat State
	FORCEINLINE ECombatState GetCombatState() const { return CombatState; }
	FORCEINLINE void SetCombatState(ECombatState State) { CombatState = State; }

	// Overlapping Weapon
	FORCEINLINE AWeapon *GetOverlappingWeapon() { return OverlappingWeapon; }
	FORCEINLINE void SetOverlappingWeapon(AWeapon *WeaponToSet) { OverlappingWeapon = WeaponToSet; }

	// Equipped Weapon
	FORCEINLINE AWeapon *GetEquippedWeapon() { return EquippedWeapon; }
	FORCEINLINE void SetEquippedWeapon(AWeapon *WeaponToSet) { EquippedWeapon = WeaponToSet; }

	FORCEINLINE float GetAO_Yaw() const {return AO_Yaw;}
	FORCEINLINE float GetAO_Pitch() const {return AO_Pitch;}
};
