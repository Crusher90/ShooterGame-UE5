// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimMontage.h"
#include "Kismet/KismetMathLibrary.h"
#include "EnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Enemy.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Components/BoxComponent.h"
#include "ShooterCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemy::AEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	AgroBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	AgroBox->SetupAttachment(GetRootComponent());
	AgroBox->SetBoxExtent(FVector(500.f));
	AgroBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	AgroBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	AgroBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	AttackSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AttackSphere"));
	AttackSphere->SetupAttachment(GetRootComponent());
	AttackSphere->SetSphereRadius(100.f);
	AttackSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	AttackSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	AttackSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	HandBox1 = CreateDefaultSubobject<UBoxComponent>(TEXT("HandBox1"));
	HandBox1->SetupAttachment(GetMesh(), FName("RightHand"));
	HandBox1->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HandBox1->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	HandBox1->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	EnemyController = Cast<AEnemyController>(GetController());

	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	AgroBox->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnAgroBoxBeginOverlap);
	AgroBox->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnAgroBoxEndOverlap);

	AttackSphere->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnAttackSphereBeginOverlap);
	AttackSphere->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnAttackSphereEndOverlap);

	HandBox1->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnHandBox1BeginOverlap);
	HandBox1->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnHandBox1EndOverlap);

	EnemyPatrol();
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const &DamageEvent, AController *EventInstigator, AActor *DamageCauser)
{
	if (Health - DamageAmount <= 0.f)
	{
		Health = 0.f;
		Death();
	}
	else
	{
		Health -= DamageAmount;
		UAnimInstance *AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && HitReactMontage && bCanHitReact)
		{
			AnimInstance->Montage_Play(HitReactMontage, 2.5f);
			bCanHitReact = false;
			if(Health > 0)
			{
				EnemyStun();
				if (EnemyController)
				{
					EnemyController->GetBlackboardComponent()->SetValueAsBool(FName("EnemyStun"), bStunned);
				}
			}
			GetWorldTimerManager().SetTimer(HitReactTimer, this, &ThisClass::ResetHitReactValue, 1.f);
		}
	}
	return DamageAmount;
}

void AEnemy::ResetHitReactValue()
{
	bCanHitReact = true;
	bStunned = false;
	if (EnemyController)
	{
		EnemyController->GetBlackboardComponent()->SetValueAsBool(FName("EnemyStun"), bStunned);
	}
}

void AEnemy::OnAgroBoxBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (OtherActor)
	{
		ShooterCharacter = Cast<AShooterCharacter>(OtherActor);
		if (ShooterCharacter)
		{
			EnemyController->GetBlackboardComponent()->SetValueAsObject(FName("Target"), ShooterCharacter);
		}
	}
}

void AEnemy::OnAgroBoxEndOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		ShooterCharacter = Cast<AShooterCharacter>(OtherActor);
		if (ShooterCharacter)
		{
		}
	}
}

void AEnemy::OnAttackSphereBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (OtherActor)
	{
		ShooterCharacter = Cast<AShooterCharacter>(OtherActor);
		if (ShooterCharacter)
		{
			bAttack = true;
			// HandBox1->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			EnemyController->GetBlackboardComponent()->SetValueAsBool(FName("Attack"), bAttack);
		}
	}
}

void AEnemy::OnAttackSphereEndOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		ShooterCharacter = Cast<AShooterCharacter>(OtherActor);
		if (ShooterCharacter)
		{
			bAttack = false;
			// HandBox1->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			EnemyController->GetBlackboardComponent()->SetValueAsBool(FName("Attack"), bAttack);
		}
	}
}

void AEnemy::OnHandBox1BeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (OtherActor)
	{
		ShooterCharacter = Cast<AShooterCharacter>(OtherActor);
		if (ShooterCharacter && !ShooterCharacter->GetDie())
		{
			UGameplayStatics::ApplyDamage(ShooterCharacter, Damage, EnemyController, this, UDamageType::StaticClass());
		}
	}
}

void AEnemy::OnHandBox1EndOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		ShooterCharacter = Cast<AShooterCharacter>(OtherActor);
		if (ShooterCharacter)
		{
		}
	}
}

void AEnemy::EnemyPatrol()
{
	const FVector WorldPatrolPoint = UKismetMathLibrary::TransformLocation(GetActorTransform(), PatrolPoint);
	const FVector WorldPatrolPoint2 = UKismetMathLibrary::TransformLocation(GetActorTransform(), PatrolPoint2);

	DrawDebugSphere(GetWorld(), WorldPatrolPoint, 30.f, 8.f, FColor::Cyan, true);
	DrawDebugSphere(GetWorld(), WorldPatrolPoint2, 30.f, 8.f, FColor::Blue, true);

	if (EnemyController)
	{
		EnemyController->GetBlackboardComponent()->SetValueAsVector(FName("PatrolPoint"), WorldPatrolPoint);
		EnemyController->GetBlackboardComponent()->SetValueAsVector(FName("PatrolPoint2"), WorldPatrolPoint2);
	}
	EnemyController->RunBehaviorTree(GetBehaviorTree());
}

void AEnemy::EnemyStun()
{
	float Stun = FMath::RandRange(0.f, 1.f);
	if (Stun < StunChance)
	{
		bStunned = true;
	}
}

void AEnemy::PlayAttackMontage()
{
	UAnimInstance *AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AttackMontage)
	{
		int32 RandomMontage = FMath::RandRange(0, 2);
		switch (RandomMontage)
		{
		case 1:
			AnimInstance->Montage_Play(AttackMontage);
			AnimInstance->Montage_JumpToSection(FName("Attack1"));
			break;

		case 2:
			AnimInstance->Montage_Play(AttackMontage);
			AnimInstance->Montage_JumpToSection(FName("Attack2"));
			break;

		case 3:
			AnimInstance->Montage_Play(AttackMontage);
			AnimInstance->Montage_JumpToSection(FName("Attack3"));
			break;
		}
	}
}

void AEnemy::Death()
{
	bDie = true;
	bCanHitReact = false;
	UAnimInstance *AnimInstance = GetMesh()->GetAnimInstance();
	int32 SectionNumber = FMath::RandRange(0, 1);
	switch (SectionNumber)
	{
	case 0:
		if (AnimInstance && DeathMontage)
		{
			AnimInstance->Montage_Play(DeathMontage);
			AnimInstance->Montage_JumpToSection(FName("Death1"), DeathMontage);
		}
		break;

	case 1:
		if (AnimInstance && DeathMontage)
		{
			AnimInstance->Montage_Play(DeathMontage);
			AnimInstance->Montage_JumpToSection(FName("Death2"), DeathMontage);
			
		}
		break;
	}
	EnemyController->GetBlackboardComponent()->SetValueAsBool(FName("Dead"), bDie);
	FTimerHandle DestroyTimer;
	GetWorldTimerManager().SetTimer(DestroyTimer, this, &ThisClass::DestroyEnemy, 10.f);
}

void AEnemy::DestroyEnemy()
{
	Destroy();
}

void AEnemy::Destroyed()
{
	Super::Destroyed();
}
