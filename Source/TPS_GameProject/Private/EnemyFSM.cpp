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
	//�����, �Ͻ��� 
	int a = 10;
	float b = a; //�Ͻ���
	//float b = (float)a; //�����

	target = Cast<ATPS_Player>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	me = Cast<AMyEnemy>(GetOwner()); //������Ʈ�� ������ ���͸� ã�´� ã�Ƽ� �ɽ�Ʈ�Ѵ٤�

	UpdateState(EEnemyState::IDLE);
	moveSubState = EEnemyMoveState::PATROL;

	//int move = 1; // 0001
	//int idle = 4;//  0100
	//int attack = 8;//1000

	//int state = 0; //0000

	////or �� �ϳ��� ���̸� ��
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

	//�¾�� ���� �������� ���س��� ���̴�
	UpdateRandomLocation(randomLocationRadius, randomLocation);
	// ...
	// ������ �Լ����� �̷������� ����
	//if (otherActor->IsA(AEnemy::StaticClass()) {
	//	//when otherActor Enemy
	//}
	//else if (otherActor->IsA(ATPS_Player::StaticClass())) {
	//	//when otherActor player
	//}

	//������ �����ϴ� PathManager�� ã�� �ʹ�.
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

//�������� �̵�
//���ݻ��·� ����
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
	//currentTime = 0; //static ���������� ����ɶ� �ѹ��� �ʱ�ȭ �ȴ�.
	//1. �ð��� �帣�ٰ�
	currentTime += GetWorld()->GetDeltaSeconds();

	//4. ���ݵ����� �����ٸ�
	if (currentTime > attackDelay)
	{
		PRINT_LOG(TEXT("time"));
		//5. ��� ������ �Ұ����� �Ǵ��ϰ�ʹ�
		float dist = target->GetDistanceTo(me);
		// ���ݰŸ����� �־����ٸ�
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

// ������ ��ġ�� ������� �̵��ϰ� �ʹ�.
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
	////���� ���� ��ġ�� �����ߴٸ�
	//ai->
	////������ ��ġ�� ���� ��ġ�� �����ϰ� �ʹ�


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

	//���� - ���� ���� ���͸� �����
	dir = target->GetActorLocation() - me->GetActorLocation();
	//���͸� ����ȭ�ؼ� ������ ����� �����ӿ� �ִ´�

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

	//FindPathSync�� ���� ������ �����
	FAIMoveRequest requst;
	FPathFindingQuery query;

	//�����ؾ��� �������� �����
	requst.SetAcceptanceRadius(5);
	requst.SetGoalLocation(target->GetActorLocation());

	//������������ ���������� �����
	ai->BuildPathfindingQuery(requst, query);

	//������ FindPathSync�� �ִ´�
	//������ ���� FindPathSync ����� FPathFindingResult result�� �ִ´�
	FPathFindingResult result = navSys->FindPathSync(query);

	//requst�� ��û�� �����ϴٸ�
	if (result.Result == ENavigationQueryResult::Success)
	{
		//B. �� �� �ִٸ� target������ �̵�
		ai->MoveToLocation(target->GetActorLocation());
	}
	else //�Ұ����ϴٸ�
	{
		//C. �׷��� �ʴٸ� �������� ��ġ�� �ϳ� �����ؼ� �װ����� ����ʹ�
		//�¾��, �����ߴٸ�

		//ó������ beginplay���� ������ ���� �����̼����� �̵��ϰ�
		auto r = ai->MoveToLocation(randomLocation);
		// �������� ������ randomLocation�� ���� �ϸ� �ٸ� randomLocation�� �����Ѵ�
		if (r == EPathFollowingRequestResult::AlreadyAtGoal || r == EPathFollowingRequestResult::Failed)
		{
			//���� �����̼��� �����ϴ� �Լ�
			UpdateRandomLocation(randomLocationRadius, randomLocation);
			//ai->MoveToLocation(randomLocation);
		}
	}
	//D. ���� ��ġ�� ���� �ߴٸ� �ٽ� �������� ��ġ�� �缱�� �ϰ�ʹ�.
}

//���ξ� on�� �ۿ��� ȣ���ϴ� �̺�Ʈ�Լ�
void UEnemyFSM::OnDamageProcess(float damageValue)
{
	if(ai)
	{
		ai->StopMovement();
	}

	//ü���� �Ҹ��ϰ�
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
	//ü���� 0�̵Ǹ�
	//�״´�
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

	//ns->GetRandomPointInNavigableRadius(); ���������� ������ �ƹ����̳�

	//���������� ���� �� �� �ִ� ��ġ��
	bool result = ns->GetRandomReachablePointInRadius(me->GetActorLocation(), radius, navLoc);
	if(result) //����� ���̶��
	{
		outLoc = navLoc.Location; //������ randomLoaction�� �����Ѵ�
	}

	return result;
}


//3. ������ �ϰ�(���ݰŸ� �ȿ� �ִ°�)


