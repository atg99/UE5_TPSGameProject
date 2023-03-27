// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBaseComponent.h"

#include "TPS_Player.h"

// Sets default values for this component's properties
UPlayerBaseComponent::UPlayerBaseComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	bWantsInitializeComponent = true;
	// ...
}


// Called when the game starts
void UPlayerBaseComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UPlayerBaseComponent::InitializeComponent()
{
	// 비긴플레이보다 빠르게 호출됨 플레이어 이동가능 플레이어 인풋 매칭보다 빠름
	Super::InitializeComponent();


	me = Cast<ATPS_Player>(GetOwner());

	me->SetUpInputDelegate.AddUObject(this, &UPlayerBaseComponent::SetupInputComponent);

	

}

// Called every frame
void UPlayerBaseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

