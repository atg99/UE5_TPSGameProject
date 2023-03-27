// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TPS_Player.h"

// Sets default values for this component's properties
UPlayerComponent::UPlayerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPlayerComponent::BeginPlay()
{
	Super::BeginPlay();

	meComp = me->GetCharacterMovement();
	// ...
	
}


// Called every frame
void UPlayerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	FTransform trans(me->GetControlRotation());
	FVector resultDir = trans.TransformVector(me->direction);

	resultDir.Z = 0;
	resultDir.Normalize();

	me->AddMovementInput(resultDir);

	me->direction = FVector::ZeroVector;
}

void UPlayerComponent::SetupInputComponent(UInputComponent* PlayerInputComponent)
{

	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &UPlayerComponent::OnAxisHorizontal);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &UPlayerComponent::OnAxisVertical);

	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &UPlayerComponent::OnAxisLookUp);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &UPlayerComponent::OnAxisLookRight);

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &UPlayerComponent::OnActionJump);

	PlayerInputComponent->BindAction(TEXT("Run"), IE_Pressed, this, &UPlayerComponent::OnActionRunPressed);
	PlayerInputComponent->BindAction(TEXT("Run"), IE_Released, this, &UPlayerComponent::OnActionRunRelease);

	PlayerInputComponent->BindAction(TEXT("Crouch"), IE_Pressed, this, &UPlayerComponent::OnActionCrouchPressed);
	PlayerInputComponent->BindAction(TEXT("Crouch"), IE_Released, this, &UPlayerComponent::OnActionCrouchRelease);
}

void UPlayerComponent::OnAxisHorizontal(float value)
{
	me->direction.Y = value;
}

void UPlayerComponent::OnAxisVertical(float value)
{
	me->direction.X = value;
}

void UPlayerComponent::OnAxisLookUp(float value)
{
	//Pitch
	me->AddControllerPitchInput(value);
}

void UPlayerComponent::OnAxisLookRight(float value)
{
	me->AddControllerYawInput(value);
}

void UPlayerComponent::OnActionJump()
{
	me->Jump();
}

void UPlayerComponent::OnActionRunPressed()
{
	//GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_MAX);
	meComp->MaxWalkSpeed = me->runSpeed;
}

void UPlayerComponent::OnActionRunRelease()
{
	meComp->MaxWalkSpeed = me->walkSpeed;
}

void UPlayerComponent::OnActionCrouchPressed()
{
	meComp->MaxWalkSpeed = me->crouchSpeed;
	me->Crouch();
}

void UPlayerComponent::OnActionCrouchRelease()
{
	meComp->MaxWalkSpeed = me->walkSpeed;
	me->UnCrouch();
}
