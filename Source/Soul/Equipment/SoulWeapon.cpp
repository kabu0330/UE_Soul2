// Fill out your copyright notice in the Description page of Project Settings.


#include "SoulWeapon.h"

#include "SGameplayTagPicker.h"
#include "Kismet/GameplayStatics.h"
#include "Soul/Soul.h"
#include "Soul/Animation/SoulAnimInstance.h"
#include "Soul/Character/SoulPlayerCharacter.h"
#include "Soul/Components/CombatComponent.h"
#include "Soul/Components/WeaponCollisionComponent.h"
#include "Soul/Data/MontageActionData.h"
#include "Soul/Data/WeaponStatData.h"


ASoulWeapon::ASoulWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
	
	WeaponCollisionComponent = CreateDefaultSubobject<UWeaponCollisionComponent>("WeaponCollisionComponent");
	WeaponCollisionComponent->OnHitActor.AddUObject(this, &ThisClass::OnHitActor); // 델리게이트
	
	SecondWeaponCollisionComponent = CreateDefaultSubobject<UWeaponCollisionComponent>("SecondWeaponCollisionComponent");
	SecondWeaponCollisionComponent->OnHitActor.AddUObject(this, &ThisClass::OnHitActor);
}

void ASoulWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASoulWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float ASoulWeapon::GetAttackDamage() const
{
	check(CombatComponent);
	ensure(WeaponStatDataAsset);

	const FGameplayTag LastAttackType = CombatComponent->GetLastAttackType();
	
	if (WeaponStatDataAsset->DamageMultiplierMap.Contains(LastAttackType))
	{
		const float Multiplier = WeaponStatDataAsset->DamageMultiplierMap[LastAttackType];
		return WeaponStatDataAsset->BaseDamage * Multiplier;
	}
	else
	{
		LOG_WARNING("무기 스탯 데이터 에셋에 데미지 배율이 등록되지 않은 공격입니다: %s, %s", *this->GetName(), *LastAttackType.ToString());
	}
	
	return WeaponStatDataAsset->BaseDamage;
}

void ASoulWeapon::OnHitActor(const FHitResult& Hit)
{
	AActor* TargetActor = Hit.GetActor();
	
	const FVector DamageDirection = GetOwner()->GetActorForwardVector();
	const float AttackDamage = GetAttackDamage();
	LOG_WARNING("Hit Actor: %s, Damage : %f", *TargetActor->GetName(), AttackDamage);

	UGameplayStatics::ApplyPointDamage(
		TargetActor,
		AttackDamage,
		DamageDirection,  // 피해가 적용된 방향, 피격 당한 액터가 넉백되거나 피격 효과를 출력할 때 유용
		Hit,
		GetOwner()->GetInstigatorController(), // 피해를 일으킨 컨트롤러, 어그로 관리나 점수 처리 등 판단
		this, // 피해를 일으킨 액터, 주로 무기나 발사체
		nullptr // 피해 유형을 정의하는 클래스, 화염 냉기 관통 등 속성 데미지
		);
}

void ASoulWeapon::EquipItem()
{
	Super::EquipItem();
	check(WeaponCollisionComponent);
	
	// 픽업 아이템에서 장비를 스폰할 때 Owner를 파라미터로 넣어줬다.
	if (ASoulPlayerCharacter* PlayerCharacter = Cast<ASoulPlayerCharacter>(GetOwner()))
	{
		if (UCombatComponent* CombatComp = PlayerCharacter->GetCombatComponent())
		{
			CombatComponent = CombatComp;
		}
	}

	// 전투 상태에 따라 소켓을 달리 적용
	const FName AttachSocket = CombatComponent->IsCombatEnabled() ? EquipSocketName : UnequipSocketName;
	AttachToOwner(AttachSocket);

	// 컴뱃 컴포넌트에 무기를 알려주고 기존에 무기를 장착하고 있다면 교체
	CombatComponent->SetWeapon(this);

	// 무기 메시를 알려줌으로써 무기 메시의 소켓을 통한 충돌 판정 길이를 알게 된다.
	WeaponCollisionComponent->SetWeaponMesh(MeshComponent);
	
	// 무기 소유자는 충돌 체크에서 제외
	WeaponCollisionComponent->AddIgnoredActor(GetOwner());

	// 장착한 무기의 CombatType으로 업데이트
	if (const ASoulCharacterBase* SoulCharacter = Cast<ASoulCharacterBase>(GetOwner()))
	{
		if (USoulAnimInstance* AnimInstance = Cast<USoulAnimInstance>(SoulCharacter->GetMesh()->GetAnimInstance()))
		{
			AnimInstance->UpdateCombatMode(CombatType);
		}
	}

}

void ASoulWeapon::UnequipItem()
{
	Super::UnequipItem();
}

float ASoulWeapon::GetStaminaCost(const FGameplayTag& InTag) const
{
	ensure(WeaponStatDataAsset);

	if (WeaponStatDataAsset->StaminaCostMap[InTag])
	{
		return WeaponStatDataAsset->StaminaCostMap[InTag];
	}
	LOG_WARNING("무기 스탯 데이터 에셋에 스태미나 코스트가 등록되지 않은 공격이 실행되었습니다: %s, %s", *this->GetName(), *InTag.ToString());
	return 0.f;
}

UAnimMontage* ASoulWeapon::GetMontageForTag(const FGameplayTag& InTag, const int32 Index) const
{
	if (IsValid(MontageActionData))
	{
		return MontageActionData->GetMontageForTag(InTag, Index);
	}
	else
	{
		LOG_ERROR("무기에 몽타주 액션 데이터 에셋이 등록되지 않았습니다: %s", *this->GetName());
	}
	return nullptr;
}

void ASoulWeapon::ActivateCollision(EWeaponCollisionType InCollisionType)
{
	switch (InCollisionType)
	{
	case EWeaponCollisionType::MainCollision:
		WeaponCollisionComponent->TurnOnCollision();
		break;
	case EWeaponCollisionType::SecondCollision:
		SecondWeaponCollisionComponent->TurnOnCollision();
		break;
	}
}

void ASoulWeapon::DeactivateCollision(EWeaponCollisionType InCollisionType)
{
	switch (InCollisionType)
	{
	case EWeaponCollisionType::MainCollision:
		WeaponCollisionComponent->TurnOffCollision();
		break;
	case EWeaponCollisionType::SecondCollision:
		SecondWeaponCollisionComponent->TurnOffCollision();
		break;
	}
}

