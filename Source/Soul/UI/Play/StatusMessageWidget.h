// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StatusMessageWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class SOUL_API UStatusMessageWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void SetMessage(const FString& Message);

	UFUNCTION()
	void HideMessage();
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Message;

	FTimerHandle ClearTextTimerHandle;

};
