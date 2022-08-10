// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

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
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 150.f;
	GetCharacterMovement()->AirControl = 0.05f;
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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
	if(!bIsCrouched)
	{
		Crouch();
		return;
	}
	if(bIsCrouched)
	{
		UnCrouch();
		return;
	}
}

void AShooterCharacter::CharacterJump() 
{
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.f);
	Jump();
	FTimerHandle JumpTimer;
	GetWorldTimerManager().SetTimer(JumpTimer, this, &ThisClass::CharacterStopJump, 1.2f);
}

void AShooterCharacter::CharacterStopJump() 
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
	StopJumping();
}
