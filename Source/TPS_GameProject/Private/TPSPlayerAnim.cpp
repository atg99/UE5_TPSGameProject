// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayerAnim.h"
#include "TPS_Player.h"
#include "GameFramework/CharacterMovementComponent.h"

void UTPSPlayerAnim::NativeUpdateAnimation(float DeltaSeconds) {
	Super::NativeUpdateAnimation(DeltaSeconds);

	//onwer의 velocity를ㄹ 가져와서
	ATPS_Player* owner = Cast<ATPS_Player>(TryGetPawnOwner());
	// rightVelocity forwardVelocity를 구하고 싶다
	// vector의 내적을 이용홰서 방향값을 구하고 싶다
	if(owner == nullptr){return;}
	FVector velocity = owner->GetVelocity();
	//dot product로 계산
	rightVelocity = FVector::DotProduct(velocity,(owner->GetActorRightVector()));
	forwardVelocity = FVector::DotProduct(velocity, (owner->GetActorForwardVector()));

	isInair = owner->GetCharacterMovement()->IsFalling();

	isCrouching = owner->GetCharacterMovement()->IsCrouching();
}

void UTPSPlayerAnim::OnFire()
{
	Montage_Play(fireMontageFactory);
}
