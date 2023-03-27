// Copyright Epic Games, Inc. All Rights Reserved.


#include "TPS_GameProjectGameModeBase.h"

#include "SpawnManager.h"
#include "TPS_GameProject.h"
#include "Kismet/GameplayStatics.h"

ATPS_GameProjectGameModeBase::ATPS_GameProjectGameModeBase()
{
	UE_LOG(LogTemp, Warning, TEXT("hihihi"));
	UE_LOG(LogTemp, Warning, TEXT("헬로우")); 
	
	UE_LOG(LogTemp, Warning, TEXT("헬로우"));
	UE_LOG(LogTemp, Warning, TEXT("헬로우"));//F9 브레이크 포인트 F5 브레이크 포인트로 실행 F10 다음 선택 F5 다시 실행 

	UE_LOG(LogTemp, Warning, TEXT("%s"), *CAllINFO);

	PRINT_LOG(TEXT("%s %s"), TEXT("Hello"), TEXT("World"));

	UE_LOG(LogTemp, Warning, TEXT("%s"), *CAllINFO);
}

void ATPS_GameProjectGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	SpawnManager = Cast<ASpawnManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ASpawnManager::StaticClass()));

	//스폰매니저의 변수를 설정
	SpawnManager->makeTargetCount = level;

	LevelUp();
}

void ATPS_GameProjectGameModeBase::LevelUp()
{
	level++;

	SpawnManager->makeTargetCount = level;

	SpawnManager->makeCount = 0;

	needExp = level;
}

void ATPS_GameProjectGameModeBase::AddExp(int amount)
{
	exp += amount;

	if(exp >= needExp)
	{
		FTimerHandle TimerHandle_Up;
		//GetWorldTimerManager().SetTimer(TimerHandle_Up, this, &ATPS_GameProjectGameModeBase::LevelUpProcess, 0.01, false);
		LevelUpProcess();
	}
}

void ATPS_GameProjectGameModeBase::LevelUpProcess()
{
	exp -= needExp;
	LevelUp();
}
