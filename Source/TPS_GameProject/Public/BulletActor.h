// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BulletActor.generated.h"

UCLASS()
class TPS_GAMEPROJECT_API ABulletActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABulletActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	class AMyEnemy* enemy;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	//�浹ü, �ܰ�, ����ü������Ʈ
	UPROPERTY(EditAnywhere)
	class USphereComponent* sphereComp;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* meshComp;

	UPROPERTY(EditAnywhere) //����Ʈ ������ �����͸� �����ϴ°��� �ִٸ� �������÷��Ͱ� �������� �ʴ´�
	class UProjectileMovementComponent* movementComp;

	UPROPERTY(EditAnywhere, Category = bulletSetting)
	float bulletLifeTime = 2.0f;

	UFUNCTION()
	void OnDie();

	UFUNCTION()
	void BulletHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	/*UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);*/
};
