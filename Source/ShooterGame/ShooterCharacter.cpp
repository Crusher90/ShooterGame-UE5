// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapon.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/KismetMathLibrary.h"
#include "Animation/AnimMontage.h"
#include "ShooterPlayerController.h"
#include "ShooterHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetMesh());
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->SocketOffset = FVector(0.f, 75.f, 75.f);

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = true;

	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 150.f;
	GetCharacterMovement()->AirControl = 0.05f;
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;

	TurningInPlace = ETurningInPlace::ETIP_NotTurning;
}
  
// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (GetMesh())
	{
		GetMesh()->HideBoneByName(FName("weapon"), EPhysBodyOp::PBO_None);
		GetMesh()->HideBoneByName(FName("pistol"), EPhysBodyOp::PBO_None);
	}
	ShooterController = ShooterController == nullptr ? Cast<AShooterPlayerController>(Controller) : ShooterController;
	if (ShooterController)
	{
		ShooterController->SetHUDHealth(Health, MaxHealth);
	}
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AimOffset(DeltaTime);
	SetCrosshairToScreen(DeltaTime);
}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &ThisClass::MoveForward);
	PlayerInputComponent->BindAxis(FName("MoveRight"), this, &ThisClass::MoveRight);
	PlayerInputComponent->BindAxis(FName("LookUp"), this, &ThisClass::LookUp);
	PlayerInputComponent->BindAxis(FName("LookRight"), this, &ThisClass::LookRight);

	PlayerInputComponent->BindAction(FName("Jump"), EInputEvent::IE_Pressed, this, &ThisClass::CharacterJump);
	PlayerInputComponent->BindAction(FName("Crouch"), EInputEvent::IE_Pressed, this, &ThisClass::CrouchButtonPressed);
	PlayerInputComponent->BindAction(FName("Sprint"), EInputEvent::IE_Pressed, this, &ThisClass::Sprint);
	PlayerInputComponent->BindAction(FName("Sprint"), EInputEvent::IE_Released, this, &ThisClass::StopSprint);
	PlayerInputComponent->BindAction(FName("Equip"), EInputEvent::IE_Pressed, this, &ThisClass::EquipButtonPressed);
	PlayerInputComponent->BindAction(FName("Fire"), EInputEvent::IE_Pressed, this, &ThisClass::FireButtonPressed);
	PlayerInputComponent->BindAction(FName("Fire"), EInputEvent::IE_Released, this, &ThisClass::FireButtonReleased);
	PlayerInputComponent->BindAction(FName("Reload"), EInputEvent::IE_Pressed, this, &ThisClass::ReloadButtonPressed);
}

void AShooterCharacter::MoveForward(float Value)
{
	if (Value != 0.f && Controller != nullptr)
	{
		FRotator Rotation = Controller->GetControlRotation();
		FRotator YawRotation = FRotator(0.f, Rotation.Yaw, 0.f);
		FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		AddMovementInput(Direction, Value);
	}
}

void AShooterCharacter::MoveRight(float Value)
{
	if (Value != 0.f && Controller != nullptr)
	{
		FRotator Rotation = Controller->GetControlRotation();
		FRotator YawRotation = FRotator(0.f, Rotation.Yaw, 0.f);
		FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(Direction, Value);
	}
}

void AShooterCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value * Sensitivity * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::LookRight(float Value)
{
	AddControllerYawInput(Value * Sensitivity * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::CrouchButtonPressed()
{
	if (!bIsCrouched)
	{
		Crouch();
		return;
	}
	if (bIsCrouched)
	{
		UnCrouch();
		return;
	}
}

void AShooterCharacter::CharacterJump()
{
	Jump();
}

void AShooterCharacter::Sprint()
{
	if (bUseSprint && GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = 800.f;
	}
}

void AShooterCharacter::StopSprint()
{
	if (bUseSprint && GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
	}
}

void AShooterCharacter::EquipButtonPressed()
{
	if (OverlappingWeapon)
	{
		EquipWeapon(OverlappingWeapon);
		SetMagazineHUDAmmo();
		SetCarriedHUDAmmo();
	}
}

void AShooterCharacter::EquipWeapon(AWeapon *WeaponToEquip)
{
	if (OverlappingWeapon == nullptr)
		return;
	WeaponToEquip = OverlappingWeapon;
	if (WeaponToEquip && EquippedWeapon == nullptr)
	{
		AttachWeaponToHands(WeaponToEquip);
		SetEquippedWeapon(WeaponToEquip);
	}
	if (OverlappingWeapon && EquippedWeapon)
	{
		SwapWeapons();
	}
}

void AShooterCharacter::SwapWeapons()
{
	DropWeaponFromHands(EquippedWeapon);
	AttachWeaponToHands(OverlappingWeapon);
}

void AShooterCharacter::AttachWeaponToHands(AWeapon *WeaponInHands)
{
	if (WeaponInHands)
	{
		WeaponInHands->SetWeaponState(EWeaponState::EWS_Equipped);
		const USkeletalMeshSocket *RightHandSocket = GetMesh()->GetSocketByName(FName("RightHandSocket"));
		if (RightHandSocket)
		{
			RightHandSocket->AttachActor(WeaponInHands, GetMesh());
		}
		SetEquippedWeapon(WeaponInHands);
		CombatState = ECombatState::ECS_Occupied;
		StopSprint();
	}
}

void AShooterCharacter::DropWeaponFromHands(AWeapon *WeaponToDrop)
{
	if (WeaponToDrop)
	{
		WeaponToDrop->SetWeaponState(EWeaponState::EWS_Dropped);
		FDetachmentTransformRules DetachmentRules(EDetachmentRule::KeepWorld, true);
		WeaponToDrop->GetWeaponMesh()->DetachFromComponent(DetachmentRules);
	}
	SetEquippedWeapon(nullptr);
	CombatState = ECombatState::ECS_Unoccupied;
}

void AShooterCharacter::InitialValues()
{
	if (GetCharacterMovement())
	{
		UE_LOG(LogTemp, Warning, TEXT("InitialValues"));
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
		GetCharacterMovement()->JumpZVelocity = 600.f;
		SetUseSprint(true);
		if (EquippedWeapon)
		{
			EquippedWeapon->SetFireDelay(0.2f);
		}
	}
}

float AShooterCharacter::TakeDamage(float DamageAmount, FDamageEvent const &DamageEvent, AController *EventInstigator, AActor *DamageCauser) 
{
	if(Health - DamageAmount <= 0.f)
	{
		Health = 0.f;
	}
	else
	{
		Health -= DamageAmount;
	}
	ShooterController->SetHUDHealth(Health, MaxHealth);
	return DamageAmount;
}

void AShooterCharacter::FireButtonPressed()
{
	bFireButtonPressed = true;
	if (EquippedWeapon)
	{
		if (EquippedWeapon->GetMagazineAmmo() == 0 && EquippedWeapon->GetCarriedAmmo() == 0)
			return;

		if (EquippedWeapon->GetMagazineAmmo() == 0 && EquippedWeapon->GetCarriedAmmo() > 0)
			ReloadWeapon();

		if (bCanFire)
		{
			FHitResult HitResult;
			TraceUnderCrosshair(HitResult);
			EquippedWeapon->Fire(HitTarget);
			PlayFireRifleMontage();
			SetMagazineHUDAmmo();
			SetCarriedHUDAmmo();
			FireTimerStart();
		}
	}
}

void AShooterCharacter::FireTimerStart()
{
	if (EquippedWeapon == nullptr)
		return;
	bCanFire = false;
	GetWorldTimerManager().SetTimer(FireTimer, this, &ThisClass::FireTimerFinished, EquippedWeapon->GetFireDelay());
}

void AShooterCharacter::FireTimerFinished()
{
	bCanFire = true;
	if (EquippedWeapon->GetAutomaticWeaponValue() && bFireButtonPressed)
	{
		FireButtonPressed();
	}
}

void AShooterCharacter::ReloadButtonPressed()
{
	bCanFire = false;
	if (EquippedWeapon && EquippedWeapon->GetCarriedAmmo() == 0 && EquippedWeapon->GetMagazineAmmo() > 0)
	{
		bCanFire = true;
	}
	if (EquippedWeapon->GetCarriedAmmo() == 0 && EquippedWeapon->GetMagazineAmmo() == 0)
		return;
	if (EquippedWeapon->GetCarriedAmmo() > 0)
	{
		ReloadWeapon();
		CombatState = ECombatState::ECS_Reloading;
	}
	GetWorldTimerManager().ClearTimer(FireTimer);
}

void AShooterCharacter::ReloadWeapon()
{
	bCanFire = false;
	UAnimInstance *AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && ReloadMontage)
	{
		AnimInstance->Montage_Play(ReloadMontage);
	}
	FTimerHandle ReloadTimer;
	GetWorldTimerManager().SetTimer(ReloadTimer, this, &ThisClass::AfterReloadMontage, 2.f);
}

void AShooterCharacter::SetMagazineHUDAmmo()
{
	if (ShooterController && EquippedWeapon)
	{
		ShooterController->SetHUDMagazineAmmo(EquippedWeapon->GetMagazineAmmo());
	}
}

void AShooterCharacter::SetCarriedHUDAmmo()
{
	if (ShooterController && EquippedWeapon)
	{
		ShooterController->SetHUDCarriedAmmo(EquippedWeapon->GetCarriedAmmo());
	}
}

void AShooterCharacter::AfterReloadMontage()
{
	EquippedWeapon->Reload();
	bCanFire = true;
	CombatState = ECombatState::ECS_Occupied;
	SetMagazineHUDAmmo();
	SetCarriedHUDAmmo();
}

void AShooterCharacter::FireButtonReleased()
{
	bFireButtonPressed = false;
}

void AShooterCharacter::AimOffset(float DeltaTime)
{
	FVector Velocity = GetVelocity();
	Velocity.Z = 0.f;
	float Speed = Velocity.Size();

	bool bIsFalling = GetCharacterMovement()->IsFalling();

	if (Speed == 0.f && bIsFalling == false)
	{
		FRotator CurrentAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		FRotator DeltaAimRotation = UKismetMathLibrary::NormalizedDeltaRotator(CurrentAimRotation, StartingAimRotation);
		AO_Yaw = DeltaAimRotation.Yaw;
		if (TurningInPlace == ETurningInPlace::ETIP_NotTurning)
		{
			InterpAO_Yaw = AO_Yaw;
		}
		bUseControllerRotationYaw = true;
		TurnInPlace(DeltaTime);
	}
	if (Speed > 0.f || bIsFalling == true)
	{
		StartingAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		AO_Yaw = 0.f;
		bUseControllerRotationYaw = true;
		TurningInPlace = ETurningInPlace::ETIP_NotTurning;
	}
	AO_Pitch = GetBaseAimRotation().Pitch;
}

void AShooterCharacter::TurnInPlace(float DeltaTime)
{
	if (AO_Yaw > 90.f)
	{
		TurningInPlace = ETurningInPlace::ETIP_Right;
	}
	else if (AO_Yaw < -90.f)
	{
		TurningInPlace = ETurningInPlace::ETIP_Left;
	}
	if (TurningInPlace != ETurningInPlace::ETIP_NotTurning)
	{
		InterpAO_Yaw = FMath::FInterpTo(InterpAO_Yaw, 0.f, DeltaTime, 3.f);
		AO_Yaw = InterpAO_Yaw;
		if (FMath::Abs(AO_Yaw) < 15.f)
		{
			TurningInPlace = ETurningInPlace::ETIP_NotTurning;
			StartingAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		}
	}
}

// PlayMontages

void AShooterCharacter::PlayFireRifleMontage()
{
	if (FireRifleMontage)
	{
		UAnimInstance *AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->Montage_Play(FireRifleMontage);
		}
	}
}

void AShooterCharacter::SetCrosshairToScreen(float DeltaTime)
{
	if (Controller == nullptr)
		return;

	ShooterController = ShooterController == nullptr ? Cast<AShooterPlayerController>(Controller) : ShooterController;
	if (ShooterController)
	{
		ShooterHUD = ShooterHUD == nullptr ? Cast<AShooterHUD>(ShooterController->GetHUD()) : ShooterHUD;
		if (ShooterHUD)
		{
			FHUDPackage HUDPackage;
			if (EquippedWeapon)
			{
				HUDPackage.CrosshairCenter = EquippedWeapon->CrosshairCenter;
				HUDPackage.CrosshairLeft = EquippedWeapon->CrosshairLeft;
				HUDPackage.CrosshairRight = EquippedWeapon->CrosshairRight;
				HUDPackage.CrosshairTop = EquippedWeapon->CrosshairTop;
				HUDPackage.CrosshairBottom = EquippedWeapon->CrosshairBottom;
			}
			else
			{
				HUDPackage.CrosshairCenter = nullptr;
				HUDPackage.CrosshairLeft = nullptr;
				HUDPackage.CrosshairRight = nullptr;
				HUDPackage.CrosshairTop = nullptr;
				HUDPackage.CrosshairBottom = nullptr;
			}
			ShooterHUD->SetHUDPackage(HUDPackage);
		}
	}
}

void AShooterCharacter::TraceUnderCrosshair(FHitResult &TraceHitResult)
{
	FVector2D ViewportSize;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}
	FVector2D CrosshairLocation(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;
	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(ShooterController, CrosshairLocation, CrosshairWorldPosition, CrosshairWorldDirection);
	if (bScreenToWorld)
	{
		FVector Start = CrosshairWorldPosition;
		FVector End = Start + CrosshairWorldDirection * TRACE_LENGTH;

		GetWorld()->LineTraceSingleByChannel(TraceHitResult, Start, End, ECollisionChannel::ECC_Visibility);
		if (!TraceHitResult.bBlockingHit)
		{
			TraceHitResult.ImpactPoint = End;
			HitTarget = End;
		}
		else
		{
			HitTarget = TraceHitResult.ImpactPoint;
		}
		Enemy = Cast<AEnemy>(TraceHitResult.GetActor());
		if(Enemy)
		{
			Enemy->SetBone(TraceHitResult.BoneName.ToString());
		}
	}
}
