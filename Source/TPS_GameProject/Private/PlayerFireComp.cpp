// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerFireComp.h"
#include "BulletActor.h"
#include "MyEnemy.h"
#include "EnemyFSM.h"
#include "TPSPlayerAnim.h"
#include "TPS_Player.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"


#define GRENADE_GUN true
#define SNIPER_GUN false

// Sets default values for this component's properties
UPlayerFireComp::UPlayerFireComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


// Called when the game starts
void UPlayerFireComp::BeginPlay()
{
	Super::BeginPlay();

	crosshairUI = CreateWidget(GetWorld(), crosshairFactory);
	SniperUI = CreateWidget(GetWorld(), sniperUIFactory);
	// crosshairUI�� ȭ�鿡 ǥ���ϰ� �ʹ�.
	crosshairUI->AddToViewport();

	// 1. �¾ �� cui�� ���̰� �ϰ�ʹ�.
	// 2. �������۰��϶�  Zoomin�� �ϸ� cui X sui O
	// 3. �����϶� out cui O sui X
	// 4. Grenade�϶� 

	ChooseGun(true);
}


// Called every frame
void UPlayerFireComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPlayerFireComp::SetupInputComponent(UInputComponent* PlayerInputComponent)
{
	//����÷��̺��� ��ǲ�� ���� ����ȴ�
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &UPlayerFireComp::OnActionFireStart);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Released, this, &UPlayerFireComp::OnActionFireStop);

	PlayerInputComponent->BindAction(TEXT("weapon1"), IE_Pressed, this, &UPlayerFireComp::OnActionGrenade);
	PlayerInputComponent->BindAction(TEXT("weapon2"), IE_Pressed, this, &UPlayerFireComp::OnActionSniper);

	PlayerInputComponent->BindAction(TEXT("Zoom"), IE_Pressed, this, &UPlayerFireComp::OnActionZoomIn);
	PlayerInputComponent->BindAction(TEXT("Zoom"), IE_Released, this, &UPlayerFireComp::OnActionZoomOut);
}

void UPlayerFireComp::OnActionShotFire()
{

	APlayerCameraManager* cameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);

	//���� �̹� ��鸮�� �ִٸ� ����ϰ� �ٽ��Ѵ�
	// camshakeIns �� null �� �ƴϰ� ���� ���̶��
	/*if (camshakeIns != nullptr && !camshakeIns->IsFinished())
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("shakeRestart")));
		cameraManager->StopCameraShake(camshakeIns);
	}*/
	camshakeIns = cameraManager->StartCameraShake(camShakeAuto);

	/*loc = GetActorLocation()+ GetActorForwardVector() * 2.0f;
	GetWorld()->SpawnActor<ABulletActor>(bulletFactory, loc, GetActorRotation());*/

	auto anim = Cast<UTPSPlayerAnim>(me->GetMesh()->GetAnimInstance());
	anim->OnFire();

	//PlayAnimMontage(fireMontageFactory, 1.0f, TEXT("Default"));

	CamLinetrace();


	FVector L = me->gunMeshComp->GetSocketLocation(TEXT("FirePosition"));
	FRotator R = me->cam->GetComponentRotation();
	//�������Ʈ���� ��� ���⿡ �浹�� �Ǹ� = isHit true, ����Ʈ���̽� �浹��ġ���� ������ġ�� �����̼����� �ٲ� ���� R�� �����Ѵ�
	if (isHit) {
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("isHit")));
		R = spawnRot;
	}
	GetWorld()->SpawnActor<ABulletActor>(bulletFactory, L, R);
}

void UPlayerFireComp::ChooseGun(bool bGrenade)
{
	OnActionZoomOut();

	bChooseGrenadeGun = bGrenade;

	me->gunMeshComp->SetVisibility(bGrenade);

	me->sniperMeshComp->SetVisibility(!bGrenade);
}

void UPlayerFireComp::OnActionGrenade()
{
	ChooseGun(GRENADE_GUN);
}

void UPlayerFireComp::OnActionSniper()
{
	ChooseGun(SNIPER_GUN);
}

void UPlayerFireComp::OnActionZoomIn()
{
	if (bChooseGrenadeGun) { return; }
	isZoom = true;
	me->cam->SetFieldOfView(10);
	SniperUI->AddToViewport();
	crosshairUI->RemoveFromParent();
}

void UPlayerFireComp::OnActionZoomOut()
{
	if (bChooseGrenadeGun) { return; }
	isZoom = false;
	me->cam->SetFieldOfView(90);
	SniperUI->RemoveFromParent();
	crosshairUI->AddToViewport();
}


void UPlayerFireComp::OnActionFireStart()
{

	me->CallHit();

	if (bChooseGrenadeGun) {
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_shotDelay, this, &UPlayerFireComp::OnActionShotFire, fireInterval, true, 0);
	}
	else {

		APlayerCameraManager* cameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);

		//���� �̹� ��鸮�� �ִٸ� ����ϰ� �ٽ��Ѵ�
		// camshakeIns �� null �� �ƴϰ� ���� ���̶��
		if (camshakeIns != nullptr && !camshakeIns->IsFinished())
		{
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("shakeRestart")));
			cameraManager->StopCameraShake(camshakeIns);
		}
		camshakeIns = cameraManager->StartCameraShake(camShake);



		/*auto controller = Cast<APlayerController>(GetController());
		auto camManager = controller->PlayerCameraManager;*/

		FHitResult hitInfo;
		FVector start = me->cam->GetComponentLocation();
		FVector end = me->cam->GetForwardVector() * 10000 + start;
		FCollisionQueryParams params;
		params.AddIgnoredActor(me);

		//����Ʈ���̽� �߻�
		bool isSniperHit = GetWorld()->LineTraceSingleByChannel(hitInfo, start, end, ECollisionChannel::ECC_Visibility, params);

		//�ε�����
		if (isSniperHit) {

			FTransform trans(hitInfo.ImpactPoint);
			if(sniperImpact)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), sniperImpact, trans);
			}
			//Ư�� ����� ã�� ���� ���� �ɽ�Ʈ 
			auto hitComp = hitInfo.GetComponent();

			if (hitComp && hitComp->IsSimulatingPhysics()) {

				//���忡�� ��ŸƮ�� ���� ����ϰ� �������� ���ϰ� mass �� ���Ѵ�
				FVector force = (hitInfo.TraceEnd - hitInfo.TraceStart).GetSafeNormal() * 100000.0f * hitComp->GetMass();
				hitComp->AddForce(force);
			}

			//����Ʈ���̽��� hitinfo�� enemy���
			auto enemy = Cast<AMyEnemy>(hitInfo.GetActor());
			if (enemy != nullptr) {
				//auto fsm = enemy->myEnemyFsm;
				auto fsm = Cast<UEnemyFSM>(enemy->GetDefaultSubobjectByName(TEXT("myEnemyFsm")));

				//UEnemyFSM* enemyFsm = Cast<UEnemyFSM>(fsm)

				if (fsm->isEnemyDie)
				{
					return;
				}

				UAnimMontage* EnemyMont = enemy->enemyMontage;
				enemy->PlayAnimMontage(EnemyMont, 1, TEXT("Damage0"));

				fsm->OnDamageProcess(10.0f);

			}

		}
	}
}

void UPlayerFireComp::OnActionFireStop()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_shotDelay);
}