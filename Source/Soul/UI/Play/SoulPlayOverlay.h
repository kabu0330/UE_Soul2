// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Soul/SoulDefine.h"
#include "SoulPlayOverlay.generated.h"

class UStatBarWidget;
/**
 * 
 */
UCLASS()
class SOUL_API USoulPlayOverlay : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UStatBarWidget> StaminaStatBarWidget;

	UFUNCTION()
	void OnAttributeChanged(EAttributeType Type, float Ratio);
};
