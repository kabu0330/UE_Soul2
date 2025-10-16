// Fill out your copyright notice in the Description page of Project Settings.


#include "Soul/UI/Play/SoulPlayOverlay.h"

#include "StatBarWidget.h"
#include "StatusMessageWidget.h"
#include "Soul/Soul.h"
#include "Soul/Character/SoulCharacterBase.h"
#include "Soul/Character/SoulPlayerCharacter.h"
#include "Soul/Components/AttributeComponent.h"


void USoulPlayOverlay::NativeConstruct()
{
	Super::NativeConstruct();

	if (APawn* OwningPawn = GetOwningPlayerPawn())
	{
		if (ASoulPlayerCharacter* SoulCharacter = Cast<ASoulPlayerCharacter>(OwningPawn))
		{
			SoulCharacter->GetAttributeComponent()->OnAttributeChanged.AddDynamic(this, &ThisClass::OnAttributeChanged);

			// 바로 브로드캐스트하여 위젯에 게임 시작 직후의 상태를 위젯에 동기화
			SoulCharacter->GetAttributeComponent()->BroadCastAttributeChanged(EAttributeType::Stamina);
			
			SoulCharacter->OnStateMessage.AddDynamic(this, &ThisClass::SetStatusMessage);
			SoulCharacter->BroadcastStateMessage(TEXT(""));
		}
	}
}

void USoulPlayOverlay::OnAttributeChanged(EAttributeType Type, float Ratio)
{
	if (false == IsValid(StaminaStatBarWidget))
	{
		LOG_ERROR("SoulPlayerOverlay에 StatBarWidget이 바인딩되지 않았습니다. 위젯 블루프린트를 확인해주세요.");
		return;
	}
	
	switch (Type)
	{
	case EAttributeType::Stamina:
		StaminaStatBarWidget->SetPercent(Ratio);
		break;
	case EAttributeType::Health:
		break;
	}
}

void USoulPlayOverlay::SetStatusMessage(const FString& Message)
{
	if (IsValid(StatusMessageWidget))
	{
		StatusMessageWidget->SetMessage(Message);
	}
}
