// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "WeaponStatData.generated.h"

/**
 * 
 */
UCLASS()
class SOUL_API UWeaponStatData : public UDataAsset
{
	GENERATED_BODY()

public:
	UWeaponStatData();
	
	/** 기본 데미지 */
	UPROPERTY(EditAnywhere)
	float BaseDamage = 15.f;
	
	/** 각 무기를 장착 후 동작에 따른 스태미나 코스트 */
	UPROPERTY(EditDefaultsOnly, Category = "SoulWeapon|StaminaCost")
	TMap<FGameplayTag, float> StaminaCostMap;

	/** 데미지 승수 */
	UPROPERTY(EditAnywhere)
	TMap<FGameplayTag, float> DamageMultiplierMap;
};
