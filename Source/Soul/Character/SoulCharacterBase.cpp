// Fill out your copyright notice in the Description page of Project Settings.


#include "SoulCharacterBase.h"

#include "Components/CapsuleComponent.h"
#include "Soul/SoulGameplayTag.h"
#include "Soul/Components/AttributeComponent.h"
#include "Soul/Components/CombatComponent.h"
#include "Soul/Components/SoulCharacterMovementComponent.h"
#include "Soul/Components/StateComponent.h"
#include "Soul/Equipment/SoulWeapon.h"


ASoulCharacterBase::ASoulCharacterBase(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<USoulCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

	AttributeComponent = CreateDefaultSubobject<UAttributeComponent>("AttributeComponent");
	StateComponent = CreateDefaultSubobject<UStateComponent>("StateComponent");
	CombatComponent = CreateDefaultSubobject<UCombatComponent>("CombatComponent");

	AttributeComponent->OnDeath.AddUObject(this, &ThisClass::OnDeath);

}

void ASoulCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASoulCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASoulCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

bool ASoulCharacterBase::GetCurrentState(const FGameplayTag& InTag) const
{
	return StateComponent->IsActiveGameplayTag(InTag);
}

FGameplayTag ASoulCharacterBase::GetAttackPerform() const
{
	USoulCharacterMovementComponent* MovementComp = Cast<USoulCharacterMovementComponent>(GetCharacterMovement());
	if (IsValid(MovementComp))
	{
		// 질주 중이면 질주 공격
		if (MovementComp->IsSprinting())
		{
			return SoulGameplayTag::Character_Attack_Running;
		}
	}
	
	// 질주 이외에는 일반 공격
	return SoulGameplayTag::Character_Attack_Light;
}

bool ASoulCharacterBase::CanPerformAttack(const FGameplayTag& AttackTypeTag) const
{
	check(CombatComponent);
	check(StateComponent);
	check(AttributeComponent);

	// 공격의 기본 조건은 무기를 들고 있는가?
	if (IsValid(CombatComponent->GetMainWeapon()) == false) return false;

	// 공격을 수행할 수 없는 상태를 정의
	FGameplayTagContainer CheckTags;
	CheckTags.AddTag(SoulGameplayTag::Character_State_Rolling);

	// 여기서 공격 상태 체크를 예외로 예야 콤보 중 공격 입력이 들어올 때 즉시 다음 콤보가 동작할 수 있다.
	//CheckTags.AddTag(SoulGameplayTag::Character_State_Attacking);

	const float StaminaCost = CombatComponent->GetMainWeapon()->GetStaminaCost(AttackTypeTag);

	/** 1. 현재 상태가 공격 액션을 취할 수 있는 상태인가?
	 *  2. 무기를 손에 쥔 상태인지?
	 *  3. 공격을 할 최소한의 스태미나가 있는지?
	 */
	return StateComponent->IsAnyActiveGameplayTags(CheckTags) == false &&
			CombatComponent->IsCombatEnabled() == true &&
			AttributeComponent->CheckHasEnoughStamina(StaminaCost) == true;
}

void ASoulCharacterBase::DoAttack(const FGameplayTag& AttackTypeTag)
{
	check(StateComponent);
	check(AttributeComponent);
	check(CombatComponent);

	if (const ASoulWeapon* Weapon = CombatComponent->GetMainWeapon())
	{
		StateComponent->AddGameplayTag(SoulGameplayTag::Character_State_Attacking);
		StateComponent->ToggleMovementInput(false); // 이동 불가
		
		AttributeComponent->ToggleStaminaRegeneration(false); // 스태미나 회복 타이머 취소

		CombatComponent->SetLastAttackType(AttackTypeTag);
		int32 ComboCounter = CombatComponent->GetComboCounter();
		
		UAnimMontage* Montage = Weapon->GetMontageForTag(AttackTypeTag, ComboCounter);
		if (false == IsValid(Montage)) // 마지막 콤보 공격 이후 DoAttack이 실행된 상태면
		{
			// 콤보 공격 초기화
			CombatComponent->SetComboCounter(0);
			ComboCounter = CombatComponent->GetComboCounter();
			Montage = Weapon->GetMontageForTag(AttackTypeTag, ComboCounter);
		}

		PlayAnimMontage(Montage);

		const float StaminaCost = Weapon->GetStaminaCost(AttackTypeTag);

		// 스태미나 차감 후 추후 공격이 없으면 1.5초 후 스태미나 회복
		AttributeComponent->DecreaseStamina(StaminaCost); // 스태미나 검사는 CanPerformAttack에서 이미 하고 왔다.
		AttributeComponent->ToggleStaminaRegeneration(true, 1.5f);
	}
}

void ASoulCharacterBase::AttackFinished()
{
	if (StateComponent)
	{
		StateComponent->ToggleMovementInput(true); // 여기서 State가 Clear 된다.
		StateComponent->RemoveGameplayTag(SoulGameplayTag::Character_State_Attacking);
	}
}

void ASoulCharacterBase::OnDeath()
{
	if (UCapsuleComponent* CapsuleComp = GetCapsuleComponent())
	{
		CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	if (USkeletalMeshComponent* MeshComp = GetMesh())
	{
		// 물리 시뮬레이션에 기반한 사망 애니메이션 출력, 정상동작 안할경우 피직스 에셋의 지원여부 체크
		MeshComp->SetCollisionProfileName("Ragdoll");
		MeshComp->SetSimulatePhysics(true);
		MeshComp->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	}
}

