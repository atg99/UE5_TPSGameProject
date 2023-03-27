// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyFSM.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8 //unsigned ¾ç¼ö 8 2ÀÇ 8½Â 256
{
	IDLE,
	MOVE,
	ATTACK,
	DAMAGE,
	DIE,

};

UENUM(BlueprintType)
enum class EEnemyMoveState : uint8
{
	PATROL,
	CHASE,
	OLD_MOVE,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPS_GAMEPROJECT_API UEnemyFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FSM)
	EEnemyState mState = EEnemyState::IDLE;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FSM)
	EEnemyMoveState moveSubState;

	UFUNCTION()
	void IdleState();

	UFUNCTION()
	void MoveState();

	UFUNCTION()
	void AttackState();

	UFUNCTION()
	void DamageState();

	UFUNCTION()
	void DieState();

	UFUNCTION()
		void TickPatrol();

	UFUNCTION()
		void TickChase();

	UFUNCTION()
		void TickOldMove();

	UFUNCTION()
	void OnDamageProcess(float damageValue);

	UFUNCTION()
	void UpdateState(EEnemyState next);

	UPROPERTY()
	class ATPS_Player* target; //caching = save at global variable

	UPROPERTY()
	class AMyEnemy* me;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float currentTime = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float attackRange = 200;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isAttackPlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isDieTimer;

	UPROPERTY()
	float attackDelay = 4;

	UPROPERTY()
	FVector dir;

	UPROPERTY()
	bool isEnemyDie;

	UPROPERTY()
	class AAIController* ai;

	void OnHitEvent();

	UPROPERTY()
	FVector randomLocation;

	UPROPERTY(EditAnywhere)
	float randomLocationRadius = 500;

	UFUNCTION()
	bool UpdateRandomLocation(float radius, FVector& outLoc);

	UPROPERTY()
	class APathManager* pathManager;

	int wayIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float detectDistance = 500;

	UPROPERTY()
	float adandonDistance = 800;

};




