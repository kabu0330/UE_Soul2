// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StatBarWidget.generated.h"

class UProgressBar;
/**
 * 
 */
UCLASS()
class SOUL_API UStatBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	void SetPercent(const float Ratio) const;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> ProgressBar_Stat;
	
	
};
