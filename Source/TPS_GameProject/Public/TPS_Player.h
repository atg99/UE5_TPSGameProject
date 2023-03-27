// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//�ּ��ش�
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TPS_Player.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FSetUpInputDelegate, class UInputComponent*);

class USpringArmComponent; // ���漱��

UCLASS()
class TPS_GAMEPROJECT_API ATPS_Player : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATPS_Player();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere)
	class USpringArmComponent* springArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCameraComponent* cam;

	UPROPERTY(EditAnywhere)
	class UPlayerComponent* playerMove;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPlayerFireComp* fireComp;
	//�ٸ� Ŭ�������� ȣ���ϴ� �Լ��� on�̶�� Ű���带 ������ : �ݹ��Լ�

	FVector direction;
	FVector loc;

	float walkSpeed = 300;
	float crouchSpeed = 150;
	float runSpeed = 600;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USkeletalMeshComponent* gunMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* sniperMeshComp;

	FSetUpInputDelegate SetUpInputDelegate;

	UFUNCTION(BlueprintNativeEvent)
	void CallHit();

};
