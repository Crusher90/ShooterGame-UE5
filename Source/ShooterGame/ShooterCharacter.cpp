// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapon.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetMesh());
	CameraBoom->bUsePawnControlRotation = true;

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
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AimOffset(DeltaTime);
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
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = 600.f;
	}
}

void AShooterCharacter::StopSprint()
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
	}
}

void AShooterCharacter::EquipButtonPressed()
{
	if (OverlappingWeapon)
	{
		EquipWeapon(OverlappingWeapon);
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

void AShooterCharacter::FireButtonPressed()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->Fire();
	}
}

void AShooterCharacter::AimOffset(float DeltaTime) 
{
	FVector Velocity = GetVelocity();
	Velocity.Z = 0.f;
	float Speed = Velocity.Size();

	bool bIsFalling = GetCharacterMovement()->IsFalling();

	if(Speed == 0.f && bIsFalling == false)
	{
		FRotator CurrentAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		FRotator DeltaAimRotation = UKismetMathLibrary::NormalizedDeltaRotator(CurrentAimRotation, StartingAimRotation);
		AO_Yaw = DeltaAimRotation.Yaw;
		if(TurningInPlace == ETurningInPlace::ETIP_NotTurning)
		{
			InterpAO_Yaw = AO_Yaw;
		}
		bUseControllerRotationYaw = true;
		TurnInPlace(DeltaTime);
	}
	if(Speed > 0.f || bIsFalling == true)
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
	if(AO_Yaw > 90.f)
	{
		TurningInPlace = ETurningInPlace::ETIP_Right;
	}
	else if(AO_Yaw < -90.f)
	{
		TurningInPlace = ETurningInPlace::ETIP_Left;
	}
	if(TurningInPlace != ETurningInPlace::ETIP_NotTurning)
	{
		InterpAO_Yaw = FMath::FInterpTo(InterpAO_Yaw, 0.f, DeltaTime, 3.f);
		AO_Yaw = InterpAO_Yaw;
		if(FMath::Abs(AO_Yaw) < 15.f)
		{
			TurningInPlace = ETurningInPlace::ETIP_NotTurning;
			StartingAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		}
	}
}
