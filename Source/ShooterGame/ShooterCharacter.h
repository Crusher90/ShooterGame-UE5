// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
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
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	// Declaring SpringArmComponent
	UPROPERTY(VisibleAnywhere, Category = "ShooterCharacter")
	class USpringArmComponent *CameraBoom;

	// Declaring ComponentComponent
	UPROPERTY(VisibleAnywhere, Category = "ShooterCharacter")
	class UCameraComponent *FollowCamera;

	// Game Overall Sensitivity
	UPROPERTY(EditAnywhere, Category = "CharacterProperties")
	float Sensitivity = 45.f;

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
};
