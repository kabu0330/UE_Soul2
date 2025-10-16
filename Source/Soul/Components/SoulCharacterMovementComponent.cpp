// Fill out your copyright notice in the Description page of Project Settings.


#include "SoulCharacterMovementComponent.h"

#include "AttributeComponent.h"
#include "StateComponent.h"
#include "Soul/Soul.h"
#include "Soul/SoulGameplayTag.h"
#include "Soul/Character/SoulCharacterBase.h"
#include "Soul/Character/SoulPlayerCharacter.h"


class ASoulCharacterBase;

USoulCharacterMovementComponent::USoulCharacterMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	MaxWalkSpeed = BaseSpeed;
	BrakingDecelerationWalking = 2000.f;
}

void USoulCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	
	ASoulCharacterBase* SoulCharacter = Cast<ASoulCharacterBase>(GetOwner());
	if (IsValid(SoulCharacter))
	{
		AttributeComponent = SoulCharacter->GetAttributeComponent();
	}
}

void USoulCharacterMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                    FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

void USoulCharacterMovementComponent::Sprint()
{
	if (false == IsValid(AttributeComponent))
	{
		LOG_ERROR("캐릭터 무브먼트 컴포넌트에서 AttributeComponent를 찾지 못했습니다. : %s", *GetOwner()->GetName());
		return;
	}

	if (AttributeComponent->CheckHasEnoughStamina(5.f) && IsMovingOnGround())
	{
		// 이동속도 향상
		MaxWalkSpeed = BaseSprintSpeed;

		// 스태미나 회복 중지
		AttributeComponent->ToggleStaminaRegeneration(false);

		// 스태미나 감소
		AttributeComponent->DecreaseStamina(0.1f);
		
		bSprinting = true;

		// 상태 태그 추가
		ASoulPlayerCharacter* PlayerCharacter = Cast<ASoulPlayerCharacter>(GetOwner());
		if (IsValid(PlayerCharacter))
		{
			PlayerCharacter->GetStateComponent()->AddGameplayTag(SoulGameplayTag::Character_State_Sprinting);
		}
	}
	else
	{
		StopSprinting();
	}
}

void USoulCharacterMovementComponent::StopSprinting()
{
	// 이동속도 복원
	MaxWalkSpeed = BaseSpeed;

	// 스태미나 회복
	AttributeComponent->ToggleStaminaRegeneration(true);
	
	bSprinting = false;

	// 상태 태그 삭제
	ASoulPlayerCharacter* PlayerCharacter = Cast<ASoulPlayerCharacter>(GetOwner());
	if (IsValid(PlayerCharacter))
	{
		PlayerCharacter->GetStateComponent()->RemoveGameplayTag(SoulGameplayTag::Character_State_Sprinting);
	}
}

bool USoulCharacterMovementComponent::IsMoving() const
{
	return Velocity.Size2D() > 3.f && GetCurrentAcceleration() != FVector::Zero();
}

