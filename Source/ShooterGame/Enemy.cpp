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

	EnemyPatrol();

	EnemyController->RunBehaviorTree(GetBehaviorTree());
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) 
{
	if(Health - DamageAmount <= 0.f)
	{
		Health = 0.f;
	}
	else
	{
		Health -= DamageAmount;
	}
	UAnimInstance *AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance && HitReactMontage && bCanHitReact)
	{
		AnimInstance->Montage_Play(HitReactMontage, 2.5f);
		bCanHitReact = false;
		GetWorldTimerManager().SetTimer(HitReactTimer, this, &ThisClass::ResetHitReactValue, 1.f);
	}
	return DamageAmount;
}

void AEnemy::ResetHitReactValue() 
{
	bCanHitReact = true;
}

void AEnemy::OnAgroBoxBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult) 
{
	if(OtherActor)
	{
		ShooterCharacter = Cast<AShooterCharacter>(OtherActor);
		if(ShooterCharacter)
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
			EnemyPatrol();
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
}

