// Fill out your copyright notice in the Description page of Project Settings.


#include "Soul/UI/Play/StatBarWidget.h"

#include "Components/ProgressBar.h"

void UStatBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UStatBarWidget::SetPercent(const float Ratio) const
{
	if (ProgressBar_Stat)
	{
		ProgressBar_Stat->SetPercent(Ratio);
	}
}
