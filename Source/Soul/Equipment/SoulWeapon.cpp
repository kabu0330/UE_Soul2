// Fill out your copyright notice in the Description page of Project Settings.


#include "SoulWeapon.h"

#include "Soul/Character/SoulPlayerCharacter.h"
#include "Soul/Components/CombatComponent.h"


ASoulWeapon::ASoulWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASoulWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASoulWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASoulWeapon::EquipItem()
{
	Super::EquipItem();
	
	// 픽업 아이템에서 장비를 스폰할 때 Owner를 파라미터로 넣어줬다.
	const ASoulPlayerCharacter* PlayerCharacter = Cast<ASoulPlayerCharacter>(GetOwner());
	if (false == IsValid(PlayerCharacter)) return;
	
	UCombatComponent* CombatComp = PlayerCharacter->GetCombatComponent();
	if (false == IsValid(CombatComp)) return;

	// 컴뱃 컴포넌트에 무기를 알려주고
	// 여기서 기존에 무기를 장착하고 있다면 교체해야 한다.
	CombatComp->SetWeapon(this);

	// 전투 상태에 따라 소켓을 달리 적용
	const FName AttachSocket = CombatComp->IsCombatEnabled() ? EquipSocketName : UnequipSocketName;
	AttachToOwner(AttachSocket);
	
}

void ASoulWeapon::UnequipItem()
{
	Super::UnequipItem();
}

