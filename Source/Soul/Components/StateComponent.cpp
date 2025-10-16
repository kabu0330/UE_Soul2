// Fill out your copyright notice in the Description page of Project Settings.


#include "StateComponent.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Soul/Soul.h"
#include "Soul/SoulGameplayTag.h"


UStateComponent::UStateComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UStateComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UStateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	uint64 Index = 1000;
	GEngine->AddOnScreenDebugMessage(Index++, 10.f, FColor::Cyan, FString::Printf(TEXT("Active Gameplay Tags : ")));
	for (const FGameplayTag& GameplayTag : ActiveGameplayTags)
	{
		GEngine->AddOnScreenDebugMessage(Index++, 0.1f, FColor::Cyan, FString::Printf(TEXT("%s "), *GameplayTag.ToString()));
	}
}

void UStateComponent::AddGameplayTags(const FGameplayTagContainer& GameplayTags)
{
	for (const FGameplayTag& GameplayTag : GameplayTags)
	{
		AddGameplayTag(GameplayTag);
	}
}

void UStateComponent::AddGameplayTag(const FGameplayTag& GameplayTag)
{
	if (IsActiveGameplayTag(GameplayTag)) return;
	
	ActiveGameplayTags.AddTag(GameplayTag);
	LOG("게임 플레이 태그 추가 : %s", *GameplayTag.ToString());
}

void UStateComponent::RemoveGameplayTags(const FGameplayTagContainer& GameplayTags)
{
	for (const FGameplayTag& GameplayTag : GameplayTags)
	{
		RemoveGameplayTag(GameplayTag);
	}
}

void UStateComponent::RemoveGameplayTag(const FGameplayTag& GameplayTag)
{
	const bool Result = ActiveGameplayTags.RemoveTag(GameplayTag);
	if (false == Result)
	{
		LOG_WARNING("존재하지 않는 게임 플레이 태그 삭제 요청 : %s", *GameplayTag.ToString());
	}
	else
	{
		LOG("게임 플레이 태그 삭제 : %s", *GameplayTag.ToString());
	}
}

bool UStateComponent::IsActiveGameplayTag(const FGameplayTag& GameplayTag) const
{
	return ActiveGameplayTags.HasTagExact(GameplayTag);
}

bool UStateComponent::IsAnyActiveGameplayTags(const FGameplayTagContainer& GameplayTags) const
{
	const bool Result = ActiveGameplayTags.HasAnyExact(GameplayTags);
	return Result;
}

bool UStateComponent::IsAllActiveGameplayTags(const FGameplayTagContainer& GameplayTags) const
{
	return ActiveGameplayTags.HasAllExact(GameplayTags);
}

void UStateComponent::ClearAllGameplayTags()
{
	ActiveGameplayTags.Reset();
}

void UStateComponent::ToggleMovementInput(bool bEnabled, float Duration)
{
	if (bEnabled)
	{
		/** FLatentActionInfo는 특정 동작을 즉시 실행하지 않고 일정 시간 후나
		 * 특정 조건이 충족되었을 때 실행하도록 예약
		 * 실행 예약이 된 상태에서 다시 들어오면 예약된 동작을 취소하고 다시 동작을 예약
		*/
		
		FLatentActionInfo LatentAction;
		LatentAction.CallbackTarget = this; // 콜백 함수를 호출할 객체
		LatentAction.ExecutionFunction = "MovementInputEnableAction"; // 리플렉션으로 처리하므로 반드시 UFUNCTION 필수
		LatentAction.Linkage = 0; // 여러 개의 LatentAction을 사용할 때 사용
		LatentAction.UUID = 0; // RetriggerableDelay 동일한 UUID가 있다면 취소하고 새로 시작

		UKismetSystemLibrary::RetriggerableDelay(GetWorld(), Duration, LatentAction);
	}
	else
	{
		if (false == IsActiveGameplayTag(SoulGameplayTag::Character_State_Movement_Disabled))
		{
			AddGameplayTag(SoulGameplayTag::Character_State_Movement_Disabled);
		}
	}
}

bool UStateComponent::IsMovementInputEnabled() const
{
	return IsActiveGameplayTag(SoulGameplayTag::Character_State_Movement_Disabled);
}

void UStateComponent::MovementInputEnableAction()
{
	// 태그 삭제
	RemoveGameplayTag(SoulGameplayTag::Character_State_Movement_Disabled);
}
