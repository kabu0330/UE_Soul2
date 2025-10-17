// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_ComboWindow.h"

#include "Soul/Character/SoulCharacterBase.h"
#include "Soul/Components/CombatComponent.h"

void UAnimNotify_ComboWindow::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                          float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	if (ASoulCharacterBase* Character = Cast<ASoulCharacterBase>(MeshComp->GetOwner()))
	{
		if (UCombatComponent* CombatComp = Character->GetCombatComponent())
		{
			CombatComp->EnableComboWindow();
		}
	}
}

void UAnimNotify_ComboWindow::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (ASoulCharacterBase* Character = Cast<ASoulCharacterBase>(MeshComp->GetOwner()))
	{
		if (UCombatComponent* CombatComp = Character->GetCombatComponent())
		{
			CombatComp->DisableComboWindow();
		}
	}
}
