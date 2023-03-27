// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//최소해더
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TPS_Player.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FSetUpInputDelegate, class UInputComponent*);

class USpringArmComponent; // 전방선언

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
	//다른 클레스에서 호출하는 함수는 on이라는 키워드를 붙힌다 : 콜백함수

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
