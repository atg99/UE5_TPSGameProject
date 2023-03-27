// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyFSM.h"
#include "EnemyAnim.generated.h"

/**
 * 
 */
UCLASS()
class TPS_GAMEPROJECT_API UEnemyAnim : public UAnimInstance
{
	GENERATED_BODY()

public:

	virtual  void NativeBeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EEnemyState state;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isAttackPlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isEnemyDieEnd;

	UFUNCTION(BlueprintCallable)
	void AnimNotify_OnHit();

	UFUNCTION(BlueprintCallable)
	void AnimNotify_DieEnd();

	UPROPERTY(EditAnywhere)
	class UAnimMontage* enemyMontage;

	UPROPERTY()
	class UEnemyFSM * fsm;

	UPROPERTY()
	class AMyEnemy* me;
};
