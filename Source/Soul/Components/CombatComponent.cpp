// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"

#include "Soul/Soul.h"
#include "Soul/SoulGameplayTag.h"
#include "Soul/Character/SoulCharacterBase.h"
#include "Soul/Equipment/SoulWeapon.h"
#include "Soul/Item/SoulPickupItem.h"


UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCombatComponent::SetWeapon(ASoulWeapon* NewWeapon)
{
	// 이미 무기를 장착 중인 상태라면 기존 무기를 다시 월드에 스폰해서 떨군다.
	if (IsValid(MainWeapon))
	{
		if (ASoulCharacterBase* CharacterBase = Cast<ASoulCharacterBase>(GetOwner()))
		{
			ASoulPickupItem* PickupItem = GetWorld()->SpawnActorDeferred<ASoulPickupItem>(
				ASoulPickupItem::StaticClass(), CharacterBase->GetTransform(), nullptr, nullptr,
				ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);

			// 픽업 아이템이 표시할 BP 클래스가 무엇인지 알려주기 위해 스폰 액터 디퍼드 사용 
			PickupItem->SetEquipmentClass(MainWeapon->GetClass());
			
			PickupItem->FinishSpawning(CharacterBase->GetTransform());

			MainWeapon->Destroy(); // nullptr 처리
		}
	}

	// 무기 설정
	MainWeapon = NewWeapon;
}

void UCombatComponent::Attack()
{
	const ASoulCharacterBase* CharacterBase = Cast<ASoulCharacterBase>(GetOwner());
	if (false == IsValid(CharacterBase)) return;

	// 스프린트 상태에 따라 다른 결과 반환
	const FGameplayTag AttackTypeTag = CharacterBase->GetAttackPerform();
	if (CharacterBase->CanPerformAttack(AttackTypeTag))
	{
		ExecuteComboAttack(AttackTypeTag);
	}
}

void UCombatComponent::SpecialAttack()
{
	const ASoulCharacterBase* CharacterBase = Cast<ASoulCharacterBase>(GetOwner());
	if (false == IsValid(CharacterBase)) return;
	
	const FGameplayTag AttackTypeTag = SoulGameplayTag::Character_Attack_Special;
	if (CharacterBase->CanPerformAttack(AttackTypeTag))
	{
		ExecuteComboAttack(AttackTypeTag);
	}
}

void UCombatComponent::HeavyAttack()
{
	const ASoulCharacterBase* CharacterBase = Cast<ASoulCharacterBase>(GetOwner());
	if (false == IsValid(CharacterBase)) return;
	
	const FGameplayTag AttackTypeTag = SoulGameplayTag::Character_Attack_Heavy;
	if (CharacterBase->CanPerformAttack(AttackTypeTag))
	{
		ExecuteComboAttack(AttackTypeTag);
	}
}

void UCombatComponent::ExecuteComboAttack(const FGameplayTag& AttackTypeTag)
{
	// 해당 함수에 들어오기 전에 공격을 실행할 수 없는 조건들은 모두 검사했다. CanPerformAttack 함수에서 
	const ASoulCharacterBase* CharacterBase = Cast<ASoulCharacterBase>(GetOwner());
	if (false == IsValid(CharacterBase)) return;

	// 공격 애니메이션이 종료된 상태에서
	if (false == CharacterBase->GetCurrentState(SoulGameplayTag::Character_State_Attacking))
	{
		// 애니메이션은 끝났지만, 콤보시퀀스 변수가 true인 시간에는 추가 입력 기회를 준다.
		if (bComboSequenceRunning == true && bCanComboInput == false)
		{
			++ComboCounter;
			LOG_WARNING("추가 공격 입력 기회 : Combo counter : %d", ComboCounter);
		}
		else // 첫 번째 공격
		{
			LOG(">>> 콤보 시퀀스 시작 <<<");
			ResetCombo();
			bComboSequenceRunning = true;
		}

		DoAttack(AttackTypeTag);

		GetWorld()->GetTimerManager().ClearTimer(ComboResetTimerHandle);
	}
	// 아직 공격 애니메이션이 끝나지 않았는데 콤보 입력이 추가로 들어온 경우 : 최적의 타이밍
	else if (bCanComboInput) 
	{
		LOG_WARNING("Combo Hit!!!!");
		bSavedComboInput = true;
	}
}

void UCombatComponent::DoAttack(const FGameplayTag& AttackTypeTag)
{
	ASoulCharacterBase* CharacterBase = Cast<ASoulCharacterBase>(GetOwner());
	if (false == IsValid(CharacterBase)) return;

	// 각종 컴포넌트를 알아야해서 캐릭터에서 나머지 로직을 처리하도록 다시 보낸다.
	CharacterBase->DoAttack(AttackTypeTag);
}

void UCombatComponent::ResetCombo()
{
	LOG_WARNING("콤보 리셋");

	bComboSequenceRunning = false;
	bCanComboInput = false;
	bSavedComboInput = false;
	ComboCounter = 0;
}

void UCombatComponent::AttackFinished(const float ComboResetDelay)
{
	// 추가 입력 시간은 AnimNotify_AttackComboFinished에서 설정한다.
	
	// ComboResetDelay 간 추가 콤보 입력 시간을 준 뒤 콤보 시퀀스 종료
	// ComboResetDelay가 0이면 즉시 콤보 종료
	GetWorld()->GetTimerManager().SetTimer(ComboResetTimerHandle, this, &UCombatComponent::ResetCombo, ComboResetDelay);
	
	ASoulCharacterBase* CharacterBase = Cast<ASoulCharacterBase>(GetOwner());
	if (false == IsValid(CharacterBase)) return;
	
	// 상태 처리를 위해 캐릭터로 내려 보낸다.
	CharacterBase->AttackFinished();
}

void UCombatComponent::EnableComboWindow()
{
	bCanComboInput = true;
	LOG_WARNING("Combo Window Opened : Combo counter : %d", ComboCounter);
}

void UCombatComponent::DisableComboWindow()
{
	bCanComboInput = false;

	// 애니메이션 재생 중에 다음 공격 입력이 들어왔다면 다음 애니메이션 재생 준비
	if (bSavedComboInput)
	{
		bSavedComboInput = false; 
		++ComboCounter;
		LOG_WARNING("Combo Window Closed : 입력 확인, 다음 콤보 실행");
		DoAttack(LastAttackType); // DoAttack에서 LastAttackType을 갱신하고 있다.
	}
	else
	{
		LOG_WARNING("Combo Window Closed : 입력 없음, 추가 입력 기회 제공");
	}
}





