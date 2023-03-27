// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnManager.h"
#include "MyEnemy.h"
#include "SpawnPoint.h"
#include "Kismet/GameplayStatics.h"
#include "TPS_GameProject/TPS_GameProjectGameModeBase.h"

// Sets default values
ASpawnManager::ASpawnManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASpawnManager::BeginPlay()
{
	Super::BeginPlay();

	//timerloop가 true면 createTime을 바꿀수없다

	TArray<AActor*> spawnPoints;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnPoint::StaticClass(), spawnPoints);

	for (auto spawnPoint : spawnPoints)
	{
		spawnVec.Add(spawnPoint->GetActorLocation());
	}

	createTime = FMath::RandRange(minTime, maxTime);
	GetWorldTimerManager().SetTimer(TimerHandle_MakeEnemy, this, &ASpawnManager::MakeEnemy, createTime, false);
}

// Called every frame
void ASpawnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpawnManager::MakeEnemy()
{

	if(makeCount < makeTargetCount)
	{

		makeCount++;

		int index = FMath::RandRange(0, spawnVec.Num() - 1);

		if (index == prevRandIndex)
		{
			index = (index + 1) % spawnVec.Num();
		}

		/*while(true)
		{
			if(spawnVec.Num() <= 2)
			{
				break;
			}

			if (index == prevRandIndex)
			{
				index = FMath::RandRange(0, spawnVec.Num() - 1);
			}
			else
			{
				break;
			}
		}*/

		prevRandIndex = index;

		FVector loc = spawnVec[index];

		int32 i = FMath::RandRange(0, 100);

		Cast<ATPS_GameProjectGameModeBase>(GetWorld()->GetAuthGameMode())->level;

		rate += Cast<ATPS_GameProjectGameModeBase>(GetWorld()->GetAuthGameMode())->level*10;

		if(i > rate)
		{
			i = 0;
		}
		else
		{
			i = 1;
		}
		GetWorld()->SpawnActor<AMyEnemy>(enemyFactorys[i], loc, GetActorRotation());
	}

	createTime = FMath::RandRange(minTime, maxTime);

	GetWorldTimerManager().SetTimer(TimerHandle_MakeEnemy, this, &ASpawnManager::MakeEnemy, createTime, false);
}

int ASpawnManager::factorial_iter(int32 n)
{
	int i, result = 1;
	for(i=1; i <= n; i++)
	{
		result = result * i;
	}
	return result;
}

