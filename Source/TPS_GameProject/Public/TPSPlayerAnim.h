// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TPSPlayerAnim.generated.h"

/**
 * 
 */
UCLASS()
class TPS_GAMEPROJECT_API UTPSPlayerAnim : public UAnimInstance
{
	GENERATED_BODY()
	
public:

	void NativeUpdateAnimation(float DeltaSeconds) override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float forwardVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float rightVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isInair;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isCrouching;

	//팩토리는 블루프린트에서 채울 정보
	UPROPERTY(EditAnywhere)
	class UAnimMontage* fireMontageFactory;

	UFUNCTION()
	void OnFire();

};
