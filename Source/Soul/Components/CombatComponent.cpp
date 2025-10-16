// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"

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

