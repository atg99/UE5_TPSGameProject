// Fill out your copyright notice in the Description page of Project Settings.


#include "MyEnemy.h"
#include "EnemyFSM.h"
#include <Components/CapsuleComponent.h>

#include "EnemyAnim.h"

// Sets default values
AMyEnemy::AMyEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh>TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Enemy/Model/vampire_a_lusth.vampire_a_lusth'"));
	if (TempMesh.Succeeded()) {
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0,0,-90), FRotator(0,-90,0));
		GetMesh()->SetRelativeScale3D(FVector(1.3f));
	}

	ConstructorHelpers::FClassFinder<UAnimInstance> tempAnim(TEXT("/Script/Engine.AnimBlueprint'/Game/BPS/ABP_EnemyAnim.ABP_EnemyAnim_C'"));

	if(tempAnim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(tempAnim.Class);
	}

	myEnemyFsm = CreateDefaultSubobject<UEnemyFSM>(TEXT("myEnemyFsm"));

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void AMyEnemy::BeginPlay()
{
	Super::BeginPlay();

	EnemyAnim = Cast<UEnemyAnim>(GetMesh()->GetAnimInstance());
}

// Called every frame
void AMyEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMyEnemy::PlayDie()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetActorLocation(GetActorLocation()+FVector(0,0,-1.0f)*GetWorld()->GetDeltaSeconds()*300.0f);
}

void AMyEnemy::PlayDieAnim()
{
	PlayAnimMontage(enemyMontage, 1, TEXT("Die"));
}

