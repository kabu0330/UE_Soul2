// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_EquipWeapon.h"

#include "Soul/SoulGameplayTag.h"
#include "Soul/Character/SoulPlayerCharacter.h"
#include "Soul/Components/CombatComponent.h"
#include "Soul/Equipment/SoulWeapon.h"

void UAnimNotify_EquipWeapon::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                     const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (const AActor* Owner = MeshComp->GetOwner())
	{
		if (const ASoulPlayerCharacter* SoulCharacter = Cast<ASoulPlayerCharacter>(Owner))
		{
			if (UCombatComponent* CombatComp = SoulCharacter->GetCombatComponent())
			{
				if (ASoulWeapon* Weapon = CombatComp->GetMainWeapon() )
				{
					bool bCombatEnabled = CombatComp->IsCombatEnabled();
					FName WeaponSocketName;

					// 애님 노티파이에서 설정한 태그에 따라 무기에서 소켓 이름 가져오기
					if (MontageActionTag == SoulGameplayTag::Character_Action_Equip)
					{
						bCombatEnabled = true;
						WeaponSocketName = Weapon->GetEquipSocketName();
					}
					else if (MontageActionTag == SoulGameplayTag::Character_Action_Unequip)
					{
						bCombatEnabled = false;
						WeaponSocketName = Weapon->GetUnequipSocketName();
					}

					// 전투 모드 변환해주고 무기 부착 위치 바꿔주고
					CombatComp->SetCombatEnabled(bCombatEnabled);
					Weapon->AttachToOwner(WeaponSocketName);
				}
			}
		}
	}
	
}
