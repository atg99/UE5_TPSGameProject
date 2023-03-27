// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayerAnim.h"
#include "TPS_Player.h"
#include "GameFramework/CharacterMovementComponent.h"

void UTPSPlayerAnim::NativeUpdateAnimation(float DeltaSeconds) {
	Super::NativeUpdateAnimation(DeltaSeconds);

	//onwer�� velocity���� �����ͼ�
	ATPS_Player* owner = Cast<ATPS_Player>(TryGetPawnOwner());
	// rightVelocity forwardVelocity�� ���ϰ� �ʹ�
	// vector�� ������ �̿�ȳ�� ���Ⱚ�� ���ϰ� �ʹ�
	if(owner == nullptr){return;}
	FVector velocity = owner->GetVelocity();
	//dot product�� ���
	rightVelocity = FVector::DotProduct(velocity,(owner->GetActorRightVector()));
	forwardVelocity = FVector::DotProduct(velocity, (owner->GetActorForwardVector()));

	isInair = owner->GetCharacterMovement()->IsFalling();

	isCrouching = owner->GetCharacterMovement()->IsCrouching();
}

void UTPSPlayerAnim::OnFire()
{
	Montage_Play(fireMontageFactory);
}
