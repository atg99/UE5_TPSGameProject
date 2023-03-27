// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnim.h"
#include "EnemyFSM.h"
#include "MyEnemy.h"


void UEnemyAnim::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	me = Cast<AMyEnemy>(TryGetPawnOwner());

	fsm = Cast<UEnemyFSM>(me->GetDefaultSubobjectByName(TEXT("myEnemyFsm")));

}

void UEnemyAnim::AnimNotify_OnHit()
{
	isAttackPlay = false;
	if(this != nullptr)
	{
		fsm->OnHitEvent();
	}
	
}

void UEnemyAnim::AnimNotify_DieEnd()
{
	//Montage_Pause(enemyMontage);
	isEnemyDieEnd = true;
}
