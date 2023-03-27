// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnManager.generated.h"

UCLASS()
class TPS_GAMEPROJECT_API ASpawnManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	//일정시간마다 적을 내위치에 생성하고 싶다
	// -일정시간
	UPROPERTY(EditAnywhere)
	float minTime = 0.5;

	float maxTime = 2;

	float createTime;
	// -타이머 핸들
	FTimerHandle TimerHandle_MakeEnemy;
	// -적공장
	/*UPROPERTY(EditAnywhere, Category=Factory)
	TSubclassOf<class AMyEnemy> enemyFactory;*/

	UPROPERTY(EditAnywhere, Category = Factory)
	TArray<TSubclassOf<class AMyEnemy>> enemyFactorys;

	void MakeEnemy();

	UPROPERTY()
	TArray<FVector> spawnVec;

	int32 prevRandIndex = -1;

	//만든 갯수
	int makeCount;
	//목표 갯수
	int makeTargetCount;

	int32 rate = 0;

	int factorial_iter(int32 n);
};
