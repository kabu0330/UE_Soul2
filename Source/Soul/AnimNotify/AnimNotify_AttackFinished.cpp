// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_AttackFinished.h"

#include "Soul/Character/SoulCharacterBase.h"
#include "Soul/Components/CombatComponent.h"

void UAnimNotify_AttackFinished::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                        const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	if (ASoulCharacterBase* Character = Cast<ASoulCharacterBase>(MeshComp->GetOwner()))
	{
		if (UCombatComponent* CombatComp = Character->GetCombatComponent())
		{
			CombatComp->AttackFinished(ComboResetDelay);
		}
	}
}
