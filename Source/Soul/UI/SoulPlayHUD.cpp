// Fill out your copyright notice in the Description page of Project Settings.


#include "Soul/UI/SoulPlayHUD.h"

#include "Play/SoulPlayOverlay.h"

ASoulPlayHUD::ASoulPlayHUD()
{
}

void ASoulPlayHUD::BeginPlay()
{
	Super::BeginPlay();

	if (PlayOverlayClass)
	{
		PlayOverlay = CreateWidget<USoulPlayOverlay>(GetWorld(), PlayOverlayClass);
		PlayOverlay->AddToViewport();
	}
}
