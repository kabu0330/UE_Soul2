// Fill out your copyright notice in the Description page of Project Settings.


#include "MontageActionData.h"

#include "Soul/Soul.h"

UAnimMontage* UMontageActionData::GetMontageForTag(const FGameplayTag& GroupTag, const int32 Index)
{
	if (MontageGroups.IsEmpty())
	{
		LOG_ERROR("몽타주 액션 데이터 에셋에 몽타주가 비어있습니다. %s", *this->GetName());
		return nullptr;
	}

	if (MontageGroups.Contains(GroupTag))
	{
		const FMontageGroup& MontageGroup = MontageGroups[GroupTag];
		if (MontageGroup.Animations.Num() > 0 && MontageGroup.Animations.Num() > Index)
		{
			return MontageGroups[GroupTag].Animations[Index];
		}

		LOG_ERROR("몽타주 액션 데이터 에셋에 등록된 인덱스를 초과한 태그 조회가 있습니다. %s, %s", *this->GetName(), *GroupTag.ToString());
	}
	else
	{
		LOG_ERROR("몽타주 액션 데이터 에셋에 존재하지 않는 태그 조회가 있습니다. : %s, %s", *this->GetName(), *GroupTag.ToString());
	}
	
	return nullptr;
}
