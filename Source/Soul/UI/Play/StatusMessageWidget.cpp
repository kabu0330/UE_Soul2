// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusMessageWidget.h"

#include "Components/TextBlock.h"

void UStatusMessageWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	HideMessage();
}

void UStatusMessageWidget::SetMessage(const FString& Message)
{
	if (TextBlock_Message)
	{
		TextBlock_Message->SetVisibility(ESlateVisibility::Visible);
		TextBlock_Message->SetText(FText::FromString(Message));

		// 3초 뒤 텍스트 숨기기
		if (GetWorld()->GetTimerManager().IsTimerActive(ClearTextTimerHandle))
		{
			GetWorld()->GetTimerManager().ClearTimer(ClearTextTimerHandle);
		}
		GetWorld()->GetTimerManager().SetTimer(ClearTextTimerHandle, this, &ThisClass::HideMessage, 3.f);
	}
}

void UStatusMessageWidget::HideMessage()
{
	if (TextBlock_Message)
	{
		TextBlock_Message->SetVisibility(ESlateVisibility::Hidden);

	}
}
