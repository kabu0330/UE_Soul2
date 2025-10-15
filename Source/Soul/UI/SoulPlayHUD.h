// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SoulPlayHUD.generated.h"

class USoulPlayOverlay;
/**
 * 
 */
UCLASS()
class SOUL_API ASoulPlayHUD : public AHUD
{
	GENERATED_BODY()

public:
	ASoulPlayHUD();

	virtual void BeginPlay() override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlueprintClass|Widget")
	TSubclassOf<USoulPlayOverlay> PlayOverlayClass;

	UPROPERTY(BlueprintReadWrite, Category = "UserWidget")
	TObjectPtr<USoulPlayOverlay> PlayOverlay;
};
