// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_WeaponCollision.h"

#include "Soul/Interface/SoulCombatInterface.h"

void UAnimNotify_WeaponCollision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                              float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (AActor* OwnerActor = MeshComp->GetOwner())
	{
		if (ISoulCombatInterface* CombatInterface = Cast<ISoulCombatInterface>(OwnerActor))
		{
			CombatInterface->ActivateCollision(CollisionType);
		}
	}
}

void UAnimNotify_WeaponCollision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (AActor* OwnerActor = MeshComp->GetOwner())
	{
		if (ISoulCombatInterface* CombatInterface = Cast<ISoulCombatInterface>(OwnerActor))
		{
			CombatInterface->DeactivateCollision(CollisionType);
		}
	}
}
