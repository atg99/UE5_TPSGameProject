// Fill out your copyright notice in the Description page of Project Settings.


#include "PathManager.h"

#include "WayPoint.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APathManager::APathManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APathManager::BeginPlay()
{
	Super::BeginPlay();

	//������ �����ϴ� WayPoint���� �� ��Ͽ� �� �־���� �ʹ�      //UClass, ����ƽ Ŭ������ �����. UCDO ������ ���� ��ü ���� ������Ʈ
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWayPoint::StaticClass(), wayPoint);

}

// Called every frame
void APathManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
