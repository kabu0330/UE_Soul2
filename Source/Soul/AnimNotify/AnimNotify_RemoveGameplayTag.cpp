// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_RemoveGameplayTag.h"

#include "Soul/Character/SoulCharacterBase.h"
#include "Soul/Components/StateComponent.h"


void UAnimNotify_RemoveGameplayTag::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                           const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	if (ASoulCharacterBase* Character = Cast<ASoulCharacterBase>(MeshComp->GetOwner()))
	{
		if (UStateComponent* StateComponent = Character->GetStateComponent())
		{
			StateComponent->RemoveGameplayTags(GameplayTags);
		}
	}
}
