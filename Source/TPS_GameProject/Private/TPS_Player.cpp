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
	//�ܰ��� �������� ���ϰ� �ʹ� ĳ���� Ŭ������ �ٷ� ���� �ִ�
	//sudo code �ǻ��ڵ� => �˰��� ( �Ӹ������� ����ϴ°ͺ��� �۷� ���°� �ſ� ȿ������ ���.
	//1. �ܰ��� �ش��ϴ� ������ �о���� �ʹ�
	//2. �о������ �����ߴٸ�
	//3. mesh�� �����ϰ� �ʹ�.
	//4. transform�� �������� ���Ѵ�
	
	// FClassFiner �������Ʈ FObjectFiner �׿� <�ڷ���,���۷��� ī�ǿ��� ã�´�>
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Quinn.SKM_Quinn'"));
             //���� �ƹ��ų�
	if (tempMesh.Succeeded()|| tempMesh.Object) {

		GetMesh()->SetSkeletalMesh(tempMesh.Object);         //FRotator pitch yaw roll, y z x 
		GetMesh()->SetRelativeLocationAndRotation(FVector(0,0,-90), FRotator(0,-90,0));
	}
	//============================================================================================

	//��������, ī�޶� ������Ʈ�� �����ϰ� �ʹ�
	//�̸��� ������ �ȵȴ�!!!
	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("springComp"));
	cam = CreateDefaultSubobject<UCameraComponent>(TEXT("camComp"));

	springArm->SetupAttachment(RootComponent);
	cam->SetupAttachment(springArm);

	springArm->SetRelativeLocation(FVector(0,50,100));
	springArm->TargetArmLength = 250;

	//�Է°��� ȸ���� �ݿ�
	bUseControllerRotationYaw = true;
	springArm->bUsePawnControlRotation = true;
	cam->bUsePawnControlRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement;

	//�Ϲ����� ������Ʈ�� ����� �ʹ�.
	gunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("gunMeshComp"));
	gunMeshComp->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));
	//�Ϲ����� ������ �о ������Ʈ�� �ְ�ʹ�.
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempGunMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));

	if (tempGunMesh.Succeeded()) {

		gunMeshComp->SetSkeletalMesh(tempGunMesh.Object);
		gunMeshComp->SetRelativeRotation(FRotator(0,0,0));
		gunMeshComp->SetRelativeLocation(FVector(0, 50, 130));
	}

	sniperMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SMP"));

	//�������Ʈ�� Ŭ���� ���δ� �ƴϸ� ������Ʈ ���δ�
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempSniper(TEXT("/Script/Engine.StaticMesh'/Game/SniperGun/sniper1.sniper1'"));

	sniperMeshComp->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));

	if (tempSniper.Succeeded()) {
		sniperMeshComp->SetStaticMesh(tempSniper.Object);
		sniperMeshComp->SetRelativeLocationAndRotation(FVector(0, 60, 140), FRotator());
		sniperMeshComp->SetRelativeScale3D(FVector(0.15f));
	}
	
	playerMove = CreateDefaultSubobject<UPlayerComponent>(TEXT("playerMove"));

	fireComp = CreateDefaultSubobject<UPlayerFireComp>(TEXT("fire"));
	//�������
	//cam->DetachFromParent();

	//ctrl+space ��ɼӼ� ��� ����
	
	//F5 ����� �������� ����

	//���������� ��Ʈ�� ���̰�
	//ī�޶�� �������Ͽ� ���̰� �ʹ�.

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

	//direction �������� �̵��ϰ� �ʹ�
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




