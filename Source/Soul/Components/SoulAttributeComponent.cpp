// Fill out your copyright notice in the Description page of Project Settings.


#include "SoulAttributeComponent.h"
#include "Soul/Soul.h"


USoulAttributeComponent::USoulAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void USoulAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void USoulAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType,
											FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	FIXED_DEBUG_MESSAGE(0, "Stamina : %f", BaseStamina);
}

bool USoulAttributeComponent::CheckHasEnoughStamina(const float Stamina) const
{
	return BaseStamina >= Stamina;
}

void USoulAttributeComponent::DecreaseStamina(const float StaminaCost)
{
	BaseStamina = FMath::Clamp(BaseStamina - StaminaCost, 0.f, MaxStamina);
}

void USoulAttributeComponent::ToggleStaminaRegeneration(bool bEnabled, float StartDelay)
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

void USoulAttributeComponent::RegenStaminaHandler()
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

void USoulAttributeComponent::BroadCastAttributeChanged(EAttributeType Type) const
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

