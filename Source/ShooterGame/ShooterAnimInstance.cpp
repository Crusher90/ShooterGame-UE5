// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterAnimInstance.h"
#include "ShooterCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Weapon.h"

void UShooterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	ShooterCharacter = ShooterCharacter == nullptr ? Cast<AShooterCharacter>(TryGetPawnOwner()) : ShooterCharacter;
}

void UShooterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	ShooterCharacter = ShooterCharacter == nullptr ? Cast<AShooterCharacter>(TryGetPawnOwner()) : ShooterCharacter;
	if (ShooterCharacter)
	{
		FVector Velocity = ShooterCharacter->GetVelocity();
		Velocity.Z = 0.f;
		Speed = Velocity.Size();

		if (ShooterCharacter && ShooterCharacter->GetCharacterMovement())
		{
			bIsFalling = ShooterCharacter->GetCharacterMovement()->IsFalling();

			if (ShooterCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f)
			{
				bIsAccelerating = true;
			}
			else
			{
				bIsAccelerating = false;
			}

			if (ShooterCharacter->GetEquippedWeapon())
			{
				bHasWeapon = true;
			}
			else
			{
				bHasWeapon = false;
			}

			bCrouched = ShooterCharacter->GetCharacterMovement()->IsCrouching();

			FRotator AimRotation = ShooterCharacter->GetBaseAimRotation();
			FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(ShooterCharacter->GetVelocity());
			FRotator DeltaRot = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation);
			DeltaRotation = FMath::RInterpTo(DeltaRotation, DeltaRot, DeltaTime, 10.f);
			YawOffset = DeltaRotation.Yaw;
			// UE_LOG(LogTemp, Warning, TEXT("OffsetYaw %f"), YawOffset);

			AO_Yaw = ShooterCharacter->GetAO_Yaw();

			AO_Pitch = ShooterCharacter->GetAO_Pitch();

			EquippedWeapon = ShooterCharacter->GetEquippedWeapon();

			if (bHasWeapon && EquippedWeapon && EquippedWeapon->GetWeaponMesh() && ShooterCharacter->GetMesh())
			{
				LeftHandTransform = EquippedWeapon->GetWeaponMesh()->GetSocketTransform(FName("LeftHandSocket"), ERelativeTransformSpace::RTS_World);
				FVector OutPosition;
				FRotator OutRotation;
				ShooterCharacter->GetMesh()->TransformToBoneSpace(FName("hand_r"), LeftHandTransform.GetLocation(), FRotator::ZeroRotator, OutPosition, OutRotation);
				LeftHandTransform.SetLocation(OutPosition);
				LeftHandTransform.SetRotation(FQuat(OutRotation));

				FTransform MuzzleTipTransform = EquippedWeapon->GetWeaponMesh()->GetSocketTransform(FName("MuzzleFlash"), ERelativeTransformSpace::RTS_World);
				FVector MuzzleX(FRotationMatrix(MuzzleTipTransform.GetRotation().Rotator()).GetUnitAxis(EAxis::X));
				// DrawDebugLine(GetWorld(), MuzzleTipTransform.GetLocation(), MuzzleTipTransform.GetLocation() + MuzzleX * 100000.f, FColor::Red);
				// DrawDebugLine(GetWorld(), MuzzleTipTransform.GetLocation(), ShooterCharacter->GetHitTarget(), FColor::Orange);

				FTransform RightHandTransform = ShooterCharacter->GetMesh()->GetSocketTransform(FName("hand_r"));
				RightHandRotation = UKismetMathLibrary::FindLookAtRotation(RightHandTransform.GetLocation(), RightHandTransform.GetLocation() + (RightHandTransform.GetLocation() - ShooterCharacter->GetHitTarget()));
			}

			TurningInPlace = ShooterCharacter->GetTurningInPlace();
			
			bUseFabrik = ShooterCharacter->GetCombatState() != ECombatState::ECS_Reloading;
		}
	}
}
