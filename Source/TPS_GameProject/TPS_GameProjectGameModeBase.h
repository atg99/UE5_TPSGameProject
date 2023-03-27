// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TPS_GameProjectGameModeBase.generated.h"

/**
 * 
 */
UCLASS()//�𸮾󿡼� �ص� ���� �Լ� �̾Ƴ�
class TPS_GAMEPROJECT_API ATPS_GameProjectGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ATPS_GameProjectGameModeBase();

	virtual void BeginPlay() override;

	UPROPERTY()
	class ASpawnManager* SpawnManager;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 level =1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 exp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 needExp = 1;

	void LevelUp();

	void AddExp(int amount);

	void LevelUpProcess();
};
