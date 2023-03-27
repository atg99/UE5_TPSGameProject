// Fill out your copyright notice in the Description page of Project Settings.


//해더중복 상관없음
#include "BulletActor.h"
#include <GameFramework/ProjectileMovementComponent.h>
#include <Components/StaticMeshComponent.h>
#include <Components/SphereComponent.h>

#include "EnemyFSM.h"
#include "../TPS_GameProject.h"
#include "MyEnemy.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABulletActor::ABulletActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("collison"));
	SetRootComponent(sphereComp);
	sphereComp->SetSphereRadius(13);
	//presset을 바꾼다
	sphereComp->SetCollisionProfileName(TEXT("BlockAll"));

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("mesh"));
	meshComp->SetupAttachment(RootComponent);
	meshComp->SetRelativeScale3D(FVector(0.25f));
	//enable설정을 바꾼다
	meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	

	movementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("move"));
	//트렌스폼이 없는 엑터 컴포넌트인 -> 무프컴포넌트가 움직일 대상을 지정해준다 디폴트는 루트
	movementComp->SetUpdatedComponent(sphereComp);

	movementComp->bShouldBounce = true;
	movementComp->Bounciness = 0.5f;
	movementComp->InitialSpeed = 5000;
	movementComp->MaxSpeed = 5000;


}

// Called when the game starts or when spawned
void ABulletActor::BeginPlay()
{
	Super::BeginPlay();
	
	FTimerHandle dieTimerHandle;
	//GetWorld()->GetTimerManager().SetTimer(dieTimerHandle, this, &ABulletActor::OnDie, 2.0f, false);
	//GetWorldTimerManager()

	//람다 함수, 구문 캡쳐 //람다는 자료형지정이 애메해서 auto
	int num = 1;
	[num]()->void {int a = num; };

	auto myPlus = [this](int a, int b)->int { return a + b; };
	PRINT_LOG(TEXT("%d"), myPlus(10, 20));

	GetWorldTimerManager().SetTimer(dieTimerHandle, FTimerDelegate::CreateLambda([this]()->void {this->Destroy();}), bulletLifeTime, false);

	//sphereComp->OnComponentBeginOverlap.AddDynamic(this, &ABulletActor::BeginOverlap);
	sphereComp->OnComponentHit.AddDynamic(this, &ABulletActor::BulletHit);

	enemy = Cast<AMyEnemy>(UGameplayStatics::GetActorOfClass(GetWorld(), AMyEnemy::StaticClass()));

}

// Called every frame
void ABulletActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABulletActor::OnDie()
{
	//UGameplayStatics::SpawnEmitterAtLocation();
	this->Destroy();
}

void ABulletActor::BulletHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if (enemy == OtherActor)
	{
		UAnimMontage* m = enemy->enemyMontage;
		enemy->PlayAnimMontage(m, 1, TEXT("Damage1"));
		auto fsm = Cast<UEnemyFSM>(enemy->GetDefaultSubobjectByName(TEXT("myEnemyFsm")));

		if(fsm->isEnemyDie)
		{
			return;
		}

		fsm->OnDamageProcess(2.0f);
	}
}

//void ABulletActor::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
//	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	if(enemy == OtherActor)
//	{
//		UAnimMontage* m = enemy->enemyMontage;
//		enemy->PlayAnimMontage(m, 1, TEXT("Damage1"));
//
//		auto fsm = Cast<UEnemyFSM>(enemy->GetDefaultSubobjectByName(TEXT("myEnemyFsm")));
//		fsm->OnDamageProcess(2.0f);
//	}
//}	

