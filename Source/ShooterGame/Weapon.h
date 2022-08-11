// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	EWS_Initial UMETA(DisplayName = "InitialState"),
	EWS_Equipped UMETA(DisplayName = "Equipped"),
	EWS_Dropped UMETA(DisplayName = "Dropped"),

	EWS_DefaultMax UMETA(DisplayName="DefaultMax")
};

UCLASS()
class SHOOTERGAME_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	// ShooterCharacter Ref Variable
	UPROPERTY(VisibleAnywhere, Category = "WeaponProperties")
	class AShooterCharacter *ShooterCharacter;

	// Skeletal WeaponMesh For Weapon
	UPROPERTY(VisibleAnywhere, Category = "WeaponProperties")
	class USkeletalMeshComponent *WeaponMesh;

	// Box Collision For Weapon
	UPROPERTY(VisibleAnywhere, Category = "WeaponProperties")
	class UBoxComponent *WeaponBox;

	// Widget Component For Pickup Weapon
	// UPROPERTY(VisibleAnywhere, Category = "WeaponProperties")
	// class UWidgetComponent *PickupWidget;

	// Decalre custom enum for WeaponState like equip drop
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "WeaponProperties", meta = (AllowPrivateAccess = "true"))
	EWeaponState WeaponState;

	// bool to decide if we overlapping with any weapon
	bool bOverlappingWeapon = false;

protected:
	// Sets the Weapon State
	void OnWeaponStateSet();

	// Sets Weapon initial State
	void WeaponInitialState();

	// Sets Weapon Equipped State
	void WeaponEquippedState();

	// Sets Weapon Dropped State
	void WeaponDroppedState();

	// Called When Begin Overlapping Weapon
	virtual void OnWeaponBoxBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	// Called When End Overlap With Weapon
	virtual void OnWeaponBoxEndOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex);

public:
	// Getters and Setters
	FORCEINLINE EWeaponState GetWeaponState() const { return WeaponState; }
	void SetWeaponState(EWeaponState State);
	
	FORCEINLINE bool GetOverlappingWeapon() const {return bOverlappingWeapon;}
};
