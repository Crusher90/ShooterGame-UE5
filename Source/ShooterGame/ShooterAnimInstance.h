// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ShooterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API UShooterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ShooterAnim", meta = (AllowPrivateAccess = "true"))
	class AShooterCharacter *ShooterCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ShooterAnim", meta = (AllowPrivateAccess = "true"))
	float Speed;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ShooterAnim", meta = (AllowPrivateAccess = "true"))
	bool bIsAccelerating;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ShooterAnim", meta = (AllowPrivateAccess = "true"))
	bool bIsFalling;
};
