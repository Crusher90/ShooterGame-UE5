// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ZombieAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API UZombieAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EnemyAnimInstance", meta = (AllowPrivateAccess = "true"))
	class AEnemy *Enemy;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EnemyAnimInstance", meta = (AllowPrivateAccess = "true"))
	float Speed;
};
