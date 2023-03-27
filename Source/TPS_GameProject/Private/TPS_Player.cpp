// Fill out your copyright notice in the Description page of Project Settings.


#include "TPS_Player.h"
#include <Camera/CameraComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include "BulletActor.h"
#include <UMG/Public/Blueprint/UserWidget.h>
#include <Kismet/GameplayStatics.h>
#include "MyEnemy.h"
#include "EnemyFSM.h"
#include "TPSPlayerAnim.h"
#include "PlayerComponent.h"
#include "PlayerFireComp.h"

#define GRENADE_GUN true
#define SNIPER_GUN false

// Sets default values
ATPS_Player::ATPS_Player()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// =======================================================================================
	//외관을 수동으로 정하고 싶다 캐릭터 클레스는 바로 쓸수 있다
	//sudo code 의사코드 => 알고리즘 ( 머리속으로 계산하는것보다 글로 쓰는게 매우 효율적인 방법.
	//1. 외관에 해당하는 에셋을 읽어오고 싶다
	//2. 읽어왔을때 성공했다면
	//3. mesh에 적용하고 싶다.
	//4. transform을 수동으로 정한다
	
	// FClassFiner 블루프린트 FObjectFiner 그외 <자료형,레퍼런스 카피에서 찾는다>
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Quinn.SKM_Quinn'"));
             //둘중 아무거나
	if (tempMesh.Succeeded()|| tempMesh.Object) {

		GetMesh()->SetSkeletalMesh(tempMesh.Object);         //FRotator pitch yaw roll, y z x 
		GetMesh()->SetRelativeLocationAndRotation(FVector(0,0,-90), FRotator(0,-90,0));
	}
	//============================================================================================

	//스프링암, 카메라 컴포넌트를 생성하고 싶다
	//이름이 같으면 안된다!!!
	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("springComp"));
	cam = CreateDefaultSubobject<UCameraComponent>(TEXT("camComp"));

	springArm->SetupAttachment(RootComponent);
	cam->SetupAttachment(springArm);

	springArm->SetRelativeLocation(FVector(0,50,100));
	springArm->TargetArmLength = 250;

	//입력값을 회전에 반영
	bUseControllerRotationYaw = true;
	springArm->bUsePawnControlRotation = true;
	cam->bUsePawnControlRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement;

	//일반총의 컴포넌트를 만들고 싶다.
	gunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("gunMeshComp"));
	gunMeshComp->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));
	//일반총의 에셋을 읽어서 컴포넌트에 넣고싶다.
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempGunMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));

	if (tempGunMesh.Succeeded()) {

		gunMeshComp->SetSkeletalMesh(tempGunMesh.Object);
		gunMeshComp->SetRelativeRotation(FRotator(0,0,0));
		gunMeshComp->SetRelativeLocation(FVector(0, 50, 130));
	}

	sniperMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SMP"));

	//블루프린트는 클레스 파인더 아니면 오브젝트 파인더
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempSniper(TEXT("/Script/Engine.StaticMesh'/Game/SniperGun/sniper1.sniper1'"));

	sniperMeshComp->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));

	if (tempSniper.Succeeded()) {
		sniperMeshComp->SetStaticMesh(tempSniper.Object);
		sniperMeshComp->SetRelativeLocationAndRotation(FVector(0, 60, 140), FRotator());
		sniperMeshComp->SetRelativeScale3D(FVector(0.15f));
	}
	
	playerMove = CreateDefaultSubobject<UPlayerComponent>(TEXT("playerMove"));

	fireComp = CreateDefaultSubobject<UPlayerFireComp>(TEXT("fire"));
	//상속해제
	//cam->DetachFromParent();

	//ctrl+space 기능속성 목록 보기
	
	//F5 디버그 에러나면 멈춤

	//스프링암을 루트에 붙이고
	//카메라는 스프링암에 붙이고 싶다.

}

// Called when the game starts or when spawned
void ATPS_Player::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed= walkSpeed;
	//SetLifeSpan(2.0f);
}

// Called every frame
void ATPS_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//direction 방향으로 이동하고 싶다
	//SetActorLocation(GetActorLocation() + direction * walkSpeed * DeltaTime);

}

// Called to bind functionality to input
void ATPS_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	SetUpInputDelegate.Broadcast(PlayerInputComponent);

	//playerMove->SetupInputComponent(PlayerInputComponent);
	//fireComp->SetupInputComponent(PlayerInputComponent);

	

	


}

void ATPS_Player::CallHit_Implementation()
{
}




