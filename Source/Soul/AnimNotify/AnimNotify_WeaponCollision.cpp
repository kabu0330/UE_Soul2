// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_WeaponCollision.h"

#include "Soul/Character/SoulPlayerCharacter.h"
#include "Soul/Components/CombatComponent.h"
#include "Soul/Components/WeaponCollisionComponent.h"
#include "Soul/Equipment/SoulWeapon.h"

void UAnimNotify_WeaponCollision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                              float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (const ASoulPlayerCharacter* SoulCharacter = Cast<ASoulPlayerCharacter>(MeshComp->GetOwner()))
	{
		if (const UCombatComponent* CombatComp = SoulCharacter->GetCombatComponent())
		{
			if (ASoulWeapon* Weapon = CombatComp->GetMainWeapon())
			{
				Weapon->GetCollision()->TurnOnCollision();
			}
		}
	}
}

void UAnimNotify_WeaponCollision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (const ASoulPlayerCharacter* SoulCharacter = Cast<ASoulPlayerCharacter>(MeshComp->GetOwner()))
	{
		if (const UCombatComponent* CombatComp = SoulCharacter->GetCombatComponent())
		{
			if (ASoulWeapon* Weapon = CombatComp->GetMainWeapon())
			{
				Weapon->GetCollision()->TurnOffCollision();
			}
		}
	}
}
