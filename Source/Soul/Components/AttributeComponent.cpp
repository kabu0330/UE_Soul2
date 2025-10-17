// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeComponent.h"

#include "StateComponent.h"
#include "Soul/Soul.h"
#include "Soul/SoulGameplayTag.h"
#include "Soul/Character/SoulCharacterBase.h"


UAttributeComponent::UAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType,
											FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	FIXED_DEBUG_MESSAGE(0, "Stamina : %f", BaseStamina);
}

bool UAttributeComponent::CheckHasEnoughStamina(const float Stamina) const
{
	return BaseStamina >= Stamina;
}

void UAttributeComponent::DecreaseStamina(const float StaminaCost)
{
	BaseStamina = FMath::Clamp(BaseStamina - StaminaCost, 0.f, MaxStamina);
}

void UAttributeComponent::ToggleStaminaRegeneration(bool bEnabled, float StartDelay)
{
	BroadCastAttributeChanged(EAttributeType::Stamina);
	
	// 회복해야 한다면
	if (bEnabled)
	{
		const bool IsActiveTimer = GetWorld()->GetTimerManager().IsTimerActive(StaminaRegenTimer);
		if (false == IsActiveTimer)
		{
			GetWorld()->GetTimerManager().SetTimer(
				StaminaRegenTimer, this, &ThisClass::RegenStaminaHandler, StaminaRegenRate, true, StartDelay);
		}
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(StaminaRegenTimer);
	}
}

void UAttributeComponent::RegenStaminaHandler()
{
	BaseStamina = FMath::Clamp(BaseStamina + StaminaRate, 0.f, MaxStamina);

	// UI 업데이트
	BroadCastAttributeChanged(EAttributeType::Stamina);

	// 스태미나가 가득차면 종료
	if (BaseStamina >= MaxStamina)
	{
		ToggleStaminaRegeneration(false);
	}
}

void UAttributeComponent::BroadCastAttributeChanged(EAttributeType Type) const
{
	if (false == OnAttributeChanged.IsBound()) return;
	
	float Ratio = 0.f;
	
	switch (Type)
	{
	case EAttributeType::Stamina:
		Ratio = BaseStamina / MaxStamina;
		break;

	case EAttributeType::Health:
		break;

	default:
		LOG_ERROR("등록되지 않은 Attribute가 브로드캐스트 되었습니다. %d", static_cast<int>(Type));
		break;
	}

	OnAttributeChanged.Broadcast(Type, Ratio);
}

void UAttributeComponent::TakeDamageAmount(float DamageAmount)
{
	BaseHealth = FMath::Clamp(BaseHealth - DamageAmount, 0.f, MaxHealth);
	BroadCastAttributeChanged(EAttributeType::Health);

	// 사망 처리 알림
	if (0.f >= BaseHealth)
	{
		if (OnDeath.IsBound())
		{
			OnDeath.Broadcast();
		}

		if (const ASoulCharacterBase* CharacterBase = Cast<ASoulCharacterBase>(GetOwner()))
		{
			if (UStateComponent* StateComp = CharacterBase->GetStateComponent())
			{
				// 사망 상태 추가
				StateComp->AddGameplayTag(SoulGameplayTag::Character_State_Death);
			}
		}
	}
}

