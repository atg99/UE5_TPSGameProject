// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"
#include <Kismet/GameplayStatics.h>
#include "TPS_Player.h"
#include "MyEnemy.h"
#include "../TPS_GameProject.h"
#include "EnemyAnim.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "PathManager.h"
#include "TPS_GameProject/TPS_GameProjectGameModeBase.h"

// Sets default values for this component's properties
UEnemyFSM::UEnemyFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEnemyFSM::BeginPlay()
{
	Super::BeginPlay();

	//mState = EEnemyState::IDLE;
	//명시적, 암시적 
	int a = 10;
	float b = a; //암시적
	//float b = (float)a; //명시적

	target = Cast<ATPS_Player>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	me = Cast<AMyEnemy>(GetOwner()); //컴포넌트를 소유한 엑터를 찾는다 찾아서 케스트한다ㅇ

	UpdateState(EEnemyState::IDLE);
	moveSubState = EEnemyMoveState::PATROL;

	//int move = 1; // 0001
	//int idle = 4;//  0100
	//int attack = 8;//1000

	//int state = 0; //0000

	////or 는 하나가 참이면 참
	//state = state | move; // 0001
	//state = state | attack;//1001

	//if((state & move) != 0)  // 0001
	//{
	//	
	//}
	//if((state & attack) != 0) //1000
	//{
	//	
	//}
	//if((state & idle) != 0) //  0000
	//{
	//	
	//}


	//TMap<FString, int> myMap;



	ai = Cast<AAIController>(me->GetController());

	//태어날때 랜덤 목적지를 정해놓을 것이다
	UpdateRandomLocation(randomLocationRadius, randomLocation);
	// ...
	// 오버랩 함수에서 이런식으로 검증
	//if (otherActor->IsA(AEnemy::StaticClass()) {
	//	//when otherActor Enemy
	//}
	//else if (otherActor->IsA(ATPS_Player::StaticClass())) {
	//	//when otherActor player
	//}

	//레벨에 존재하는 PathManager를 찾고 싶다.
	pathManager = Cast<APathManager>(UGameplayStatics::GetActorOfClass(GetWorld(), APathManager::StaticClass()));
}


// Called every frame
void UEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	/*auto enemyAnim = Cast<UEnemyAnim>(me->EnemyAnim);
	enemyAnim->state = mState;*/

	switch (mState)
	{
	case  EEnemyState::IDLE:
		IdleState();
		break;
	case  EEnemyState::MOVE:
		MoveState();
		break;
	case  EEnemyState::ATTACK:
		AttackState();
		break;
	case  EEnemyState::DAMAGE:
		DamageState();
		break;
	case  EEnemyState::DIE:
		DieState();
		break;
	}
	// ...
}

/// <summary>
/// three slash will write to document
/// </summary>
void UEnemyFSM::IdleState()
{
	//1. find player
	//2. if find player
	//3. change movestate
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("idle")));
	if (target != nullptr) {
		UpdateState(EEnemyState::MOVE);
	}
}

//목적지로 이동
//공격상태로 전이
void UEnemyFSM::MoveState()
{
	switch (moveSubState)
	{
	case  EEnemyMoveState::PATROL:
		TickPatrol();
		break;
	case  EEnemyMoveState::CHASE:
		TickChase();
		break;
	case  EEnemyMoveState::OLD_MOVE:
		TickOldMove();
		break;
	}

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("move")));

}

void UEnemyFSM::AttackState()
{
	//currentTime = 0; //static 정적변수는 실행될때 한번만 초기화 된다.
	//1. 시간이 흐르다가
	currentTime += GetWorld()->GetDeltaSeconds();

	//4. 공격동작이 끝났다면
	if (currentTime > attackDelay)
	{
		PRINT_LOG(TEXT("time"));
		//5. 계속 공격을 할것인지 판단하고싶다
		float dist = target->GetDistanceTo(me);
		// 공격거리보다 멀어졌다면
		if (dist > attackRange) {
			PRINT_LOG(TEXT("out"));
			currentTime = 0;
			me->EnemyAnim->isAttackPlay=false;
			UpdateState(EEnemyState::MOVE);
			
		}
		else {
			PRINT_LOG(TEXT("in"));
			me->EnemyAnim->isAttackPlay = true;
			currentTime = 0;
		}
	}
}

//damaged
void UEnemyFSM::DamageState()
{
	currentTime += GetWorld()->GetDeltaSeconds();
	if (currentTime > 2) {
		UpdateState(EEnemyState::MOVE);
		currentTime = 0;
	}
}

void UEnemyFSM::DieState()
{
	if(!(me->EnemyAnim->isEnemyDieEnd))
	{
		return;
	}
	currentTime += GetWorld()->GetDeltaSeconds();
	if (currentTime > 1) {
		isDieTimer = true;
		currentTime = 0;
	}
	if (isDieTimer) {
		me->PlayDie();
	}
}

// 순찰할 위치를 순서대로 이동하고 싶다.
void UEnemyFSM::TickPatrol()
{

	//TArray<>;
	////TMap;
	////TSet;

	FVector patrolTarget = pathManager->wayPoint[wayIndex]->GetActorLocation();
	auto result = ai->MoveToLocation(patrolTarget, 100);

	/*wayIndex = (wayIndex + 1) % pathManager->wayPoint.Num();

	wayIndex = (wayIndex + 1 + pathManager->wayPoint.Num()) % pathManager->wayPoint.Num();*/

	if(result == EPathFollowingRequestResult::AlreadyAtGoal || result == EPathFollowingRequestResult::Failed)
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("patrol At Goal")));

		wayIndex++;
		if(wayIndex >= pathManager->wayPoint.Num())
		{
			wayIndex = 0;
		}

	}
	////만약 순찰 위치에 도착했다면
	//ai->
	////순찰할 위치를 다음 위치로 갱신하고 싶다


	//UKismetSystemLibrary::CapsuleOverlapActors(GetWorld(), me->GetActorLocation(), 100, 10);
	//me->GetDistanceTo(target);
	FVector dis = target->GetActorLocation() - me->GetActorLocation();
	if(dis.Size() <= detectDistance)
	{
		moveSubState = EEnemyMoveState::CHASE;
	}

}

void UEnemyFSM::TickChase()
{
	ai->MoveToLocation(target->GetActorLocation());

	//목적 - 시작 으로 백터를 만들고
	dir = target->GetActorLocation() - me->GetActorLocation();
	//백터를 정규화해서 방향을 만들고 움직임에 넣는다

	//me->AddMovementInput(dir.GetSafeNormal());

	float dist = 0;
	//float dist = target->GetDistanceTo(me);
	//if(dir.Size != nullptr)
	//{
	dist = dir.Size();
	//}
	//float dist = FVector::Dist(target->GetActorLocation(), me->GetActorLocation());

	if (dist <= attackRange) {
		UpdateState(EEnemyState::ATTACK);
	}
	else if(dist > adandonDistance)
	{
		moveSubState = EEnemyMoveState::PATROL;
	}
}

void UEnemyFSM::TickOldMove()
{
	UNavigationSystemV1* navSys = UNavigationSystemV1::GetNavigationSystem(GetWorld());

	//FindPathSync에 넣을 변수를 만든다
	FAIMoveRequest requst;
	FPathFindingQuery query;

	//도달해야할 목적지를 만든다
	requst.SetAcceptanceRadius(5);
	requst.SetGoalLocation(target->GetActorLocation());

	//목적지에대한 쿼리변수를 만든다
	ai->BuildPathfindingQuery(requst, query);

	//쿼리를 FindPathSync에 넣는다
	//쿼리에 대한 FindPathSync 결과를 FPathFindingResult result에 넣는다
	FPathFindingResult result = navSys->FindPathSync(query);

	//requst의 요청이 가능하다면
	if (result.Result == ENavigationQueryResult::Success)
	{
		//B. 갈 수 있다면 target쪽으로 이동
		ai->MoveToLocation(target->GetActorLocation());
	}
	else //불가능하다면
	{
		//C. 그렇지 않다면 무작위로 위치를 하나 선정해서 그곳으로 가고싶다
		//태어날때, 도착했다면

		//처음에는 beginplay에서 설정한 랜덤 로케이션으로 이동하고
		auto r = ai->MoveToLocation(randomLocation);
		// 무작위로 선정한 randomLocation에 도달 하면 다른 randomLocation을 설정한다
		if (r == EPathFollowingRequestResult::AlreadyAtGoal || r == EPathFollowingRequestResult::Failed)
		{
			//랜덤 로케이션을 설정하는 함수
			UpdateRandomLocation(randomLocationRadius, randomLocation);
			//ai->MoveToLocation(randomLocation);
		}
	}
	//D. 만약 위치에 도달 했다면 다시 무작위로 위치를 재선정 하고싶다.
}

//접두어 on은 밖에서 호출하는 이벤트함수
void UEnemyFSM::OnDamageProcess(float damageValue)
{
	if(ai)
	{
		ai->StopMovement();
	}

	//체력을 소모하고
	if(me->HP <= 0)
	{
		return;
	}
	me->HP -= damageValue;
	if (me->HP <= 0) {

		isEnemyDie = true;

		me->EnemyAnim->isEnemyDieEnd = false;
		me->PlayDieAnim();
		UpdateState(EEnemyState::DIE);
		//addExp
		Cast<ATPS_GameProjectGameModeBase>(GetWorld()->GetAuthGameMode())->AddExp(1);
	}
	else {
		UpdateState(EEnemyState::DAMAGE);
	}
	//체력이 0이되면
	//죽는다
}

void UEnemyFSM::UpdateState(EEnemyState next)
{
	currentTime = 0;
	mState = next;
	if (me->EnemyAnim != nullptr)
	{
		me->EnemyAnim->state = next;
	}
}

void UEnemyFSM::OnHitEvent()
{
	if (this != nullptr){
		float dist = target->GetDistanceTo(me);
		if (dist <= attackRange) {
			PRINT_LOG(TEXT("Enemy is Attack"));
		}
	}
}

bool UEnemyFSM::UpdateRandomLocation(float radius, FVector& outLoc)
{
	UNavigationSystemV1* ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());

	FNavLocation navLoc;

	//ns->GetRandomPointInNavigableRadius(); 범위내에서 길위의 아무곳이나

	//범위내에서 도달 할 수 있는 위치만
	bool result = ns->GetRandomReachablePointInRadius(me->GetActorLocation(), radius, navLoc);
	if(result) //결과가 참이라면
	{
		outLoc = navLoc.Location; //참조로 randomLoaction을 변경한다
	}

	return result;
}


//3. 공격을 하고(공격거리 안에 있는가)


