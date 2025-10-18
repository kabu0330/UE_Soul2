// Fill out your copyright notice in the Description page of Project Settings.


#include "SoulFistWeapon.h"

#include "Soul/Animation/SoulAnimInstance.h"
#include "Soul/Character/SoulPlayerCharacter.h"
#include "Soul/Components/CombatComponent.h"
#include "Soul/Components/WeaponCollisionComponent.h"


class ASoulPlayerCharacter;

ASoulFistWeapon::ASoulFistWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASoulFistWeapon::EquipItem()
{
	check(WeaponCollisionComponent);
	
	// 픽업 아이템에서 장비를 스폰할 때 Owner를 파라미터로 넣어줬다.
	ASoulPlayerCharacter* OwnerCharacter = Cast<ASoulPlayerCharacter>(GetOwner());
	if (false == IsValid(OwnerCharacter)) return;

	if (UCombatComponent* CombatComp = OwnerCharacter->GetCombatComponent())
	{
		CombatComponent = CombatComp;
	}

	// 전투 상태에 따라 소켓을 달리 적용
	const FName AttachSocket = CombatComponent->IsCombatEnabled() ? EquipSocketName : UnequipSocketName;
	AttachToOwner(AttachSocket);

	// 컴뱃 컴포넌트에 무기를 알려주고 기존에 무기를 장착하고 있다면 교체
	CombatComponent->SetWeapon(this);

	// 주먹을 메시로 사용하기 때문에 캐릭터의 스켈레탈 메시로 설정해준다.
	WeaponCollisionComponent->SetWeaponMesh(OwnerCharacter->GetMesh());
	SecondWeaponCollisionComponent->SetWeaponMesh(OwnerCharacter->GetMesh());
	
	// 무기 소유자는 충돌 체크에서 제외
	WeaponCollisionComponent->AddIgnoredActor(GetOwner());
	SecondWeaponCollisionComponent->AddIgnoredActor(GetOwner());

	// 주먹은 언제든지 공격할 수 있어야 하니까 장착/탈착 애니메이션이 필요없다.
	CombatComponent->SetCombatEnabled(true);

	// 장착한 무기의 CombatType으로 업데이트
	if (const ASoulCharacterBase* SoulCharacter = Cast<ASoulCharacterBase>(GetOwner()))
	{
		if (USoulAnimInstance* AnimInstance = Cast<USoulAnimInstance>(SoulCharacter->GetMesh()->GetAnimInstance()))
		{
			AnimInstance->UpdateCombatMode(CombatType);
		}
	}
}






