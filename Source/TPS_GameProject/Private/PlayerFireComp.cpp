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
	// crosshairUI를 화면에 표시하고 싶다.
	crosshairUI->AddToViewport();

	// 1. 태어날 때 cui를 보이게 하고싶다.
	// 2. 스나이퍼건일때  Zoomin을 하면 cui X sui O
	// 3. 스나일때 out cui O sui X
	// 4. Grenade일때 

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
	//비긴플레이보다 인풋이 먼저 실행된다
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

	//만약 이미 흔들리고 있다면 취소하고 다시한다
	// camshakeIns 가 null 이 아니고 흔드는 중이라면
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
	//블루프린트에서 쏘는 방향에 충돌이 되면 = isHit true, 라인트레이스 충돌위치에서 시작위치를 로테이션으로 바꾼 값을 R에 대입한다
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

		//만약 이미 흔들리고 있다면 취소하고 다시한다
		// camshakeIns 가 null 이 아니고 흔드는 중이라면
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

		//라인트레이스 발사
		bool isSniperHit = GetWorld()->LineTraceSingleByChannel(hitInfo, start, end, ECollisionChannel::ECC_Visibility, params);

		//부딪히면
		if (isSniperHit) {

			FTransform trans(hitInfo.ImpactPoint);
			if(sniperImpact)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), sniperImpact, trans);
			}
			//특정 대상을 찾고 싶을 때는 케스트 
			auto hitComp = hitInfo.GetComponent();

			if (hitComp && hitComp->IsSimulatingPhysics()) {

				//엔드에서 스타트를 빼서 노멀하고 스케일을 곱하고 mass 를 곱한다
				FVector force = (hitInfo.TraceEnd - hitInfo.TraceStart).GetSafeNormal() * 100000.0f * hitComp->GetMass();
				hitComp->AddForce(force);
			}

			//라인트레이스가 hitinfo가 enemy라면
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