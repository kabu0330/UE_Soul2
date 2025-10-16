// Fill out your copyright notice in the Description page of Project Settings.


#include "SoulAnimInstance.h"

#include "KismetAnimationLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Soul/Character/SoulCharacterBase.h"
#include "Soul/Components/StateComponent.h"

USoulAnimInstance::USoulAnimInstance()
{
}

void USoulAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Character = Cast<ACharacter>(GetOwningActor());
	if (Character)
	{
		MovementComponent = Character->GetCharacterMovement();
	}
}

void USoulAnimInstance::NativeUpdateAnimation(float DeltaSecond)
{
	Super::NativeUpdateAnimation(DeltaSecond);

	if (!IsValid(Character)) return;
	if (!IsValid(MovementComponent)) return;

	Velocity = MovementComponent->Velocity;
	GroundSpeed = Velocity.Size2D(); // 점프는 필요 없으니까 z축 빼고 x,y만 계산

	// Modules : AnimGraphRuntime (UKismetAnimationLibrary)
	Direction = UKismetAnimationLibrary::CalculateDirection(Velocity, Character->GetActorRotation());

	bShouldMove = GroundSpeed > 3.0f && MovementComponent->GetCurrentAcceleration() != FVector::Zero();
	bIsFalling = MovementComponent->IsFalling();
	
	
}

void USoulAnimInstance::AnimNotify_ResetMovementInput()
{
	if (ASoulCharacterBase* SoulCharacter = Cast<ASoulCharacterBase>(GetOwningActor()))
	{
		SoulCharacter->GetStateComponent()->ToggleMovementInput(true);
	}
}

