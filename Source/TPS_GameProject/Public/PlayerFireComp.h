// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBaseComponent.h"
#include "PlayerFireComp.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPS_GAMEPROJECT_API UPlayerFireComp : public UPlayerBaseComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerFireComp();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	virtual void SetupInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, Category = PlayerSetting) //�����Ϳ��� ���� ����ȭ
	float fireInterval = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTimerHandle TimerHandle_shotDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerSetting)
	bool bChooseGrenadeGun;

	//�������忡�� ������ �����Ѵ�. Crosshair, Sniper
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerSetting)
	TSubclassOf<class UUserWidget> crosshairFactory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=PlayerSetting)
	TSubclassOf<class UUserWidget> sniperUIFactory;

	UPROPERTY() //����Ʈ������ ��������Ƽ ����ȭ�� �Ǹ鼭 ī������ �Ѵ� , UPROPERTY�� ������ Null�̵ȴ�
	class UUserWidget* crosshairUI;

	UPROPERTY() //����Ʈ������ ��������Ƽ ����ȭ�� �Ǹ鼭 ī������ �Ѵ� , UPROPERTY�� ������ Null�̵ȴ�
	class UUserWidget* SniperUI;



	void OnActionFireStart();
	void OnActionFireStop();
	UFUNCTION(BlueprintCallable)
	void OnActionShotFire();

	UFUNCTION()
	void ChooseGun(bool bGrenade); 

	UFUNCTION()
	void OnActionGrenade();

	UFUNCTION()
	void OnActionSniper();

	UFUNCTION()
	void OnActionZoomIn();

	UFUNCTION()
	void OnActionZoomOut();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isZoom = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator spawnRot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isHit=false;
	
	UFUNCTION(BlueprintImplementableEvent)
	void CamLinetrace();

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCameraShakeBase> camShake;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCameraShakeBase> camShakeAuto;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABulletActor> bulletFactory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UParticleSystem* sniperImpact;

protected:
	UPROPERTY()
	class UCameraShakeBase* camshakeIns;
};
