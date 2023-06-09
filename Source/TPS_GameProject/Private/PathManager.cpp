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

	//레벨에 존재하는 WayPoint들을 내 목록에 다 넣어놓고 싶다      //UClass, 스테틱 클레스로 만든다. UCDO 실제로 만들어낸 객체 서브 오브젝트
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWayPoint::StaticClass(), wayPoint);

}

// Called every frame
void APathManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

